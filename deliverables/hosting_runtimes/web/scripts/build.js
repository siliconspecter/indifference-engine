import { favicons } from 'favicons'
import { join } from 'path'
import { promises } from 'fs'
import { optimizeZopfliPng } from 'node-zopflipng'
import { minify } from 'html-minifier'
import { minify as uglify } from 'uglify-js'
import { createHash, randomUUID } from 'crypto'
import { path7za } from '7zip-bin'
import { execFile } from 'child_process'

console.log('Cleaning build directory...')

await Promise.all((await promises.readdir('ephemeral')).filter(name => name !== '.gitignore').map(name => promises.rm(join('ephemeral', name), { recursive: true, force: true })))

console.log('Creating build directory...')

await promises.mkdir(join('ephemeral', 'build'), { recursive: true })

console.log('Generating icons and calculating hash of module.wasm...')

const faviconsConfiguration = {
  path: '/',
  appName: 'Indifference Engine',
  appShortName: 'Indifference Engine',
  appDescription: 'Small-scale, long-lived retro game engine.',
  developerName: 'siliconspecter',
  developerURL: 'https://siliconspecter.github.io/indifference-engine/',
  cacheBustingQueryParam: null,
  dir: 'auto',
  lang: 'en-US',
  background: '#000',
  theme_color: '#000',
  appleStatusBarStyle: 'black-translucent',
  display: 'standalone',
  orientation: 'landscape',
  scope: '/',
  start_url: '/',
  preferRelatedApplications: false,
  relatedApplications: undefined,
  version: '1.0',
  pixel_art: true,
  loadManifestWithCredentials: false,
  manifestMaskable: false
}

const [icons, logos, wasmModuleHash] = await Promise.all([
  favicons(join('source', 'logo.gif'), {
    ...faviconsConfiguration,
    icons: {
      android: true,
      appleIcon: true,
      appleStartup: false,
      favicons: true,
      windows: true,
      yandex: true
    }
  }),
  favicons(join('source', 'logo.gif'), {
    ...faviconsConfiguration,
    icons: {
      android: false,
      appleIcon: false,
      appleStartup: true,
      favicons: false,
      windows: false,
      yandex: false
    }
  }),
  (async () => {
    const wasmModuleContent = await promises.readFile(join('..', '..', 'wasm_module', 'ephemeral', 'production', 'build', 'module.wasm'))
    const hash = createHash('sha256')
    hash.update(wasmModuleContent)
    return hash.digest('hex')
  })()
])

console.log('Compressing and writing files...')

await Promise.all([
  promises.copyFile(
    join('..', '..', 'wasm_module', 'ephemeral', 'production', 'build', 'module.wasm'),
    join('ephemeral', 'build', `module-${wasmModuleHash}.wasm`)
  ),
  (async () => {
    const fileNames = await promises.readdir(join('..', '..', 'wasm_module', 'ephemeral', 'production', 'build'))

    const index = fileNames.indexOf('wasm.wasm')

    if (index !== -1) {
      fileNames.splice(index, 1)
    }

    await Promise.all(fileNames.map(fileName => promises.copyFile(join('..', '..', 'wasm_module', 'ephemeral', 'production', 'build', fileName), join('ephemeral', 'build', fileName))))
  })(),
  ...[icons, logos]
    .map(set => set.images)
    .flat()
    .map(async image => {
      const contents = image.name.endsWith('.png')
        ? (await optimizeZopfliPng(image.contents, {
            lossyTransparent: true,
            lossy8bit: true,
            iterations: 15
          }))
        : image.contents
      await promises.writeFile(join('ephemeral', 'build', image.name), contents)
    }),
  ...[icons, logos]
    .map(set => set.files)
    .flat()
    .map(async file => {
      await promises.writeFile(join('ephemeral', 'build', file.name), file.name.endsWith('.json') ? JSON.stringify(JSON.parse(file.contents)) : file.contents, 'utf-8')
    }),
  (async () => {
    const rawJavascript = await promises.readFile(join('source', 'service-worker.js'), 'utf-8')

    const injectedJavascript = rawJavascript
      .replace(/INSERT-CACHE-PREFIX-HERE/g, 'indifference-engine')
      .replace(/INSERT-CACHE-BUSTER-HERE/g, randomUUID())
      .replace(/INSERT-WASM-MODULE-PATH-HERE/g, `module-${wasmModuleHash}.wasm`)

    const uglifiedJavascript = uglify(injectedJavascript, {
      compress: {
        toplevel: true,
        keep_fargs: false,
        unsafe_comps: true,
        unsafe_Function: true,
        unsafe_math: true,
        unsafe_proto: true,
        unsafe_regexp: true,
        unsafe_undefined: true
      },
      mangle: {
        eval: true,
        toplevel: true
      },
      toplevel: true
    })

    if (uglifiedJavascript.error !== undefined) {
      throw uglifiedJavascript.error
    }

    const hash = createHash('sha256')
    hash.update(uglifiedJavascript.code, 'utf-8')
    const uglifiedJavascriptHash = hash.digest('hex')

    await Promise.all([
      promises.writeFile(join('ephemeral', 'build', `service-worker-${uglifiedJavascriptHash}.js`), uglifiedJavascript.code, 'utf-8'),
      (async () => {
        const rawHtml = await promises.readFile(join('source', 'index.html'), 'utf-8')

        const injectedHtml = rawHtml
          .replace('<!-- INSERT-FAVICONS-GENERATED-HTML-HERE -->', [logos, icons].map(set => set.html).flat().join(''))
          .replace('INSERT-WASM-MODULE-PATH-HERE', `module-${wasmModuleHash}.wasm`)
          .replace('INSERT-SERVICE-WORKER-JS-PATH-HERE', `service-worker-${uglifiedJavascriptHash}.js`)
          .replace(/INSERT-LOCAL-STORAGE-PREFIX-HERE/g, 'indifference-engine')

        const minifiedHtml = minify(injectedHtml, {
          caseSensitive: false,
          collapseBooleanAttributes: true,
          collapseInlineTagWhitespace: true,
          collapseWhitespace: true,
          conservativeCollapse: false,
          continueOnParseError: false,
          decodeEntities: true,
          html5: true,
          includeAutoGeneratedTags: false,
          keepClosingSlash: false,
          minifyCSS: true,
          minifyJS: true,
          minifyURLs: true,
          preserveLineBreaks: false,
          preventAttributesEscaping: false,
          processConditionalComments: true,
          removeAttributeQuotes: true,
          removeComments: true,
          removeEmptyAttributes: true,
          removeEmptyElements: false,
          removeOptionalTags: true,
          removeRedundantAttributes: true,
          removeScriptTypeAttributes: true,
          removeStyleLinkTypeAttributes: true,
          removeTagWhitespace: true,
          sortAttributes: true,
          sortClassName: true,
          trimCustomFragments: true,
          useShortDoctype: true
        })

        await promises.writeFile(join('ephemeral', 'build', 'index.html'), minifiedHtml, 'utf-8')
      })()
    ])
  })()
])

console.log('Zipping...')

const stats = await promises.stat(path7za)

const executeBits = 73

if ((stats.mode & executeBits) !== executeBits) {
  await promises.chmod(path7za, stats.mode | executeBits)
}

await new Promise((resolve, reject) => {
  execFile(path7za, [
    'a',
    '-mm=Deflate',
    '-mfb=258',
    '-mpass=15',
    '-r',
    join('ephemeral', 'build.zip'),
    join('ephemeral', 'build', '*')
  ])
    .on('error', reject)
    .on('exit', (code) => {
      if (code === 0) {
        resolve()
      } else {
        reject(new Error(`Failed to zip; 7za exit code ${code}.`))
      }
    })
})

console.log('Done.')
