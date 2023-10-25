const cachePrefix = 'INSERT-CACHE-PREFIX-HERE-'
const cacheName = `${cachePrefix}INSERT-CACHE-BUSTER-HERE`

self.addEventListener('install', async (event) => {
  self.skipWaiting()
  event.waitUntil((await self.caches.open(cacheName)).addAll([
    'INSERT-MODULE-WASM-PATH-HERE'
  ]))
})

self.addEventListener('activate', async (event) => {
  event.waitUntil(Promise.all((await self.caches.keys()).filter(key => key.startsWith(cachePrefix) && key !== cacheName).map(key => self.caches.delete(key))))
})

self.addEventListener('fetch', (event) => {
  const request = event.request

  event.respondWith((async () => {
    const cache = await self.caches.open(cacheName)

    return (await cache.match(request)) ?? (async () => {
      const response = await fetch(request)

      if (response.ok) {
        await cache.put(request, response.clone())
      }

      return response
    })()
  })())
})
