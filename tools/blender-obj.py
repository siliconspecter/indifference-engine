bl_info = {
    "name": "Indifference Engine OBJ",
    "author": "siliconspecter",
    "version": (0, 0, 0),
    "blender": (3, 6, 5),
    "location": "File > Import-Export",
    "description": "Import-Export OBJ files from/to Indifference Engine",
    "category": "Import-Export",
}

import bpy
from re import match, sub
from os import path
from bpy.utils import register_class, unregister_class
from bpy.types import (
    Operator,
    VIEW3D_MT_add,
    TOPBAR_MT_file_import,
    TOPBAR_MT_file_export,
    Menu,
)
from bpy_extras.io_utils import ExportHelper, ImportHelper


def initialize_scene(context):
    context.scene.render.engine = "BLENDER_EEVEE"

    for window in context.window_manager.windows:
        for area in window.screen.areas:
            if area.type == "VIEW_3D":
                for space in area.spaces:
                    if space.type == "VIEW_3D":
                        space.shading.type = "MATERIAL"


def create_material(name, texture_path):
    output = None

    for material_candidate in bpy.data.materials:
        if material_candidate.use_nodes:
            for node in material_candidate.node_tree.nodes:
                if node.type == "TEX_IMAGE":
                    if node.image is not None and node.image.filepath == texture_path:
                        output = material_candidate

    if output is None:
        if bpy.data.materials.get(name):
            index = 2

            while bpy.data.materials.get(name + "_" + str(index)):
                index += 1

            name = name + "_" + str(index)

        output = bpy.data.materials.new(name)

        output.use_nodes = True

        output_node = None

        for node in output.node_tree.nodes:
            if node.type == "OUTPUT_MATERIAL":
                output_node = node
            else:
                output.node_tree.nodes.remove(node)

        vertex_color = output.node_tree.nodes.new("ShaderNodeVertexColor")
        vertex_color.layer_name = "Attribute"
        vertex_color.location = (-500, -50)

        tex_image = output.node_tree.nodes.new("ShaderNodeTexImage")
        tex_image.image = bpy.data.images.get(texture_path) or bpy.data.images.load(
            texture_path
        )
        tex_image.extension = "CLIP"
        tex_image.interpolation = "Closest"
        tex_image.location = (-600, -225)

        mix = output.node_tree.nodes.new("ShaderNodeMix")
        mix.data_type = "RGBA"
        mix.blend_type = "MULTIPLY"
        mix.inputs[0].default_value = 1
        mix.location = (-250, 0)

        emission = output.node_tree.nodes.new("ShaderNodeEmission")
        emission.location = (0, 0)

        output_node.location = (250, 0)

        output.node_tree.links.new(mix.inputs[6], vertex_color.outputs[0])
        output.node_tree.links.new(mix.inputs[7], tex_image.outputs[0])
        output.node_tree.links.new(emission.inputs[0], mix.outputs[2])
        output.node_tree.links.new(output_node.inputs[0], emission.outputs[0])

    return output


class IndifferenceEngineOBJAddMaterial(Operator, ImportHelper):
    bl_idname = "indifference_engine_obj.add_material"
    bl_label = "Add Indifference Engine OBJ Material"
    filename_ext = ".tga"

    def execute(self, context):
        initialize_scene(context)

        name = path.basename(path.splitext(self.properties.filepath)[0])

        if not match("^[a-z_][a-z_0-9]*$", name):
            self.report(
                {"ERROR"},
                'Texture name file names should be lower case, start with a letter or underscore, followed by only letters, digits and/or underscores ("'
                + name
                + '").',
            )
            return {"FINISHED"}

        material = create_material(name, self.properties.filepath)

        for object in bpy.context.selected_objects:
            if object.type == "MESH" and not object.name.startswith("navigation_mesh_"):
                exists = False

                for index, existing_material in enumerate(object.data.materials):
                    if existing_material == material:
                        exists = True

                if not exists:
                    object.data.materials.append(material)

        return {"FINISHED"}


class IndifferenceEngineOBJAddNavigationMesh(Operator):
    bl_idname = "indifference_engine_obj.add_navigation_mesh"
    bl_label = "Add Indifference Engine OBJ Navigation Mesh"

    def execute(self, context):
        initialize_scene(context)

        # TODO

        return {"FINISHED"}


class IndifferenceEngineOBJAdd(Menu):
    bl_idname = "INDIFFERENCE_ENGINE_OBJ_MT_add"
    bl_label = "Indifference Engine"
    bl_options = {"SEARCH_ON_KEY_PRESS"}

    def draw(self, context):
        layout = self.layout
        layout.operator_context = "INVOKE_REGION_WIN"
        layout.operator(
            IndifferenceEngineOBJAddMaterial.bl_idname,
            text="Material",
            icon="MATERIAL",
        )
        layout.operator(
            IndifferenceEngineOBJAddNavigationMesh.bl_idname,
            text="Navigation Mesh",
            icon="MESH_DATA",
        )


def menu_add(self, context):
    layout = self.layout
    layout.operator_context = "INVOKE_REGION_WIN"
    layout.separator()
    layout.menu(
        IndifferenceEngineOBJAdd.bl_idname,
        text="Indifference Engine",
        icon="GHOST_ENABLED",
    )


class IndifferenceEngineOBJExport(Operator, ExportHelper):
    bl_idname = "export.indifference_engine_obj"
    bl_label = "Export Indifference Engine OBJ (*.obj)"
    filename_ext = ".obj"

    def execute(self, context):
        initialize_scene(context)

        if self.filepath.endswith(".obj"):
            mtllib = self.filepath[:-3] + "mtl"
        else:
            mtllib = self.filepath + ".mtl"

        file = None

        try:
            file = open(mtllib, "w")

            first = True

            for material in bpy.data.materials:
                if material.name == "Dots Stroke":
                    continue

                if first:
                    first = False
                else:
                    file.write("\n")

                if not match("^[a-z_][a-z_0-9]*$", material.name):
                    self.report(
                        {"ERROR"},
                        'Material names should be lower case, start with a letter or underscore, followed by only letters, digits and/or underscores ("'
                        + material.name
                        + '").',
                    )
                    return {"FINISHED"}

                file.write("newmtl " + material.name + "\n")

                if material.use_nodes:
                    for node in material.node_tree.nodes:
                        if node.type == "TEX_IMAGE":
                            if node.image is not None:
                                # TODO: Relative path
                                # TODO: Ensure TGA
                                file.write(
                                    "map_Kd "
                                    + sub(r"^/+", "", node.image.filepath)
                                    + "\n"
                                )

        finally:
            if file != None:
                file.close()

        file = None

        try:
            file = open(self.filepath, "w")

            file.write("mtllib " + path.basename(mtllib) + "\n")

            for obj in context.scene.objects:
                # TODO: throw if transformed
                # TODO: throw if modifiers

                if not match("^[a-z_][a-z_0-9]*$", obj.name):
                    self.report(
                        {"ERROR"},
                        'Object names should be lower case, start with a letter or underscore, followed by only letters, digits and/or underscores ("'
                        + obj.name
                        + '").',
                    )
                    return {"FINISHED"}

                file.write("o " + obj.name + "\n")

                if obj.type != "MESH":
                    self.report(
                        {"ERROR"},
                        'Object "' + obj.name + '" is not a mesh.',
                    )
                    return {"FINISHED"}

                mesh = obj.data

                match len(mesh.uv_layers):
                    case 0:
                        self.report(
                            {"ERROR"},
                            'Object "' + obj.name + '" does not have any UV layers.',
                        )
                        return {"FINISHED"}

                    case 1:
                        for uv_layer in mesh.uv_layers:
                            match len(mesh.vertex_colors):
                                case 0:
                                    self.report(
                                        {"ERROR"},
                                        'Object "'
                                        + obj.name
                                        + '" does not have any color layers.',
                                    )
                                    return {"FINISHED"}

                                case 1:
                                    materials = mesh.materials.values()

                                    for color_layer in mesh.vertex_colors:
                                        total_indices = 0

                                        for polygon in mesh.polygons:
                                            material = materials[polygon.material_index]

                                            if material is None:
                                                self.report(
                                                    {"ERROR"},
                                                    'Object "'
                                                    + obj.name
                                                    + '" contains faces without materials.',
                                                )
                                                return {"FINISHED"}

                                            file.write("usemtl " + material.name + "\n")

                                            for key in polygon.vertices:
                                                vertex = mesh.vertices[key]
                                                location = vertex.co
                                                uv = uv_layer.data[total_indices].uv
                                                color = color_layer.data[
                                                    total_indices
                                                ].color

                                                total_indices += 1

                                                file.write(
                                                    "v "
                                                    + f"{location[0]:f}"
                                                    + " "
                                                    + f"{location[1]:f}"
                                                    + " "
                                                    + f"{location[2]:f}"
                                                    + " "
                                                    + f"{color[0]:f}"
                                                    + " "
                                                    + f"{color[1]:f}"
                                                    + " "
                                                    + f"{color[2]:f}"
                                                    + " "
                                                    + f"{color[3]:f}"
                                                    + "\n"
                                                )

                                                file.write(
                                                    "vt "
                                                    + f"{uv[0]:f}"
                                                    + " "
                                                    + f"{uv[1]:f}"
                                                    + "\n"
                                                )

                                            file.write("f")

                                            index = -len(polygon.vertices)
                                            for key in polygon.vertices:
                                                file.write(
                                                    " " + str(index) + "/" + str(index)
                                                )
                                                index += 1

                                            file.write("\n")

                                case _:
                                    self.report(
                                        {"ERROR"},
                                        'Object "'
                                        + obj.name
                                        + '" has multiple color layers.',
                                    )
                                    return {"FINISHED"}

                    case _:
                        self.report(
                            {"ERROR"},
                            'Object "' + obj.name + '" has multiple UV layers.',
                        )
                        return {"FINISHED"}

        finally:
            if file != None:
                file.close()

        return {"FINISHED"}


def menu_export(self, context):
    self.layout.operator(IndifferenceEngineOBJExport.bl_idname)


class IndifferenceEngineOBJImport(Operator, ImportHelper):
    bl_idname = "import.indifference_engine_obj"
    bl_label = "Import Indifference Engine OBJ (*.obj)"
    filename_ext = ".obj"

    def execute(self, context):
        initialize_scene(context)

        file = None

        try:
            file = open(self.properties.filepath, "r")

            object_name = None
            vertices = []
            texture_coordinates = []
            faces = []
            materials = {}
            current_material = None
            previous_object_names = []

            def end_object():
                if object_name != None:
                    object_vertices = []
                    indices = []

                    for face in faces:
                        face_indices = []

                        for face_vertex in face:
                            vertex = vertices[face_vertex[0]]

                            index = None

                            for i, existing_vertex in enumerate(object_vertices):
                                if (
                                    existing_vertex[0] == vertex[0]
                                    and existing_vertex[1] == vertex[1]
                                    and existing_vertex[2] == vertex[2]
                                ):
                                    index = i
                                    break

                            if index == None:
                                index = len(object_vertices)
                                object_vertices.append(
                                    [vertex[0], vertex[1], vertex[2]]
                                )

                            face_indices.append(index)

                        indices.append(face_indices)

                    mesh = bpy.data.meshes.new(object_name)

                    mesh.from_pydata(
                        object_vertices,
                        [],
                        indices,
                    )

                    color_layer = mesh.vertex_colors.new(name="Attribute")
                    uv_layer = mesh.uv_layers.new(name="UV")

                    total_indices = 0

                    material_order = []

                    for face_index, face in enumerate(faces):
                        for face_vertex in face:
                            vertex = vertices[face_vertex[0]]
                            color_layer.data[total_indices].color[0] = vertex[3]
                            color_layer.data[total_indices].color[1] = vertex[4]
                            color_layer.data[total_indices].color[2] = vertex[5]
                            color_layer.data[total_indices].color[3] = vertex[6]
                            texture_coordinate = texture_coordinates[face_vertex[1]]
                            uv_layer.data[total_indices].uv[0] = texture_coordinate[0]
                            uv_layer.data[total_indices].uv[1] = texture_coordinate[1]
                            total_indices += 1

                            material = face_vertex[2]

                            if material not in material_order:
                                material_order.append(material)
                                mesh.materials.append(material)

                            mesh.polygons[
                                face_index
                            ].material_index = material_order.index(material)

                    mesh.update()

                    obj = bpy.data.objects.new(object_name, mesh)
                    context.scene.collection.objects.link(obj)

                    faces.clear()

            for line in file.readlines():
                without_comments = match("^([^#]*)#.*$", line)

                if without_comments:
                    line = without_comments[1]

                if match("\S", line):
                    object_match = match("^\s*o\s+([a-z_][a-z_0-9]*)\s*$", line)

                    if object_match:
                        end_object()

                        object_name = object_match[1]

                        if object_name in previous_object_names:
                            self.report(
                                {"ERROR"},
                                'Object "'
                                + object_name
                                + '" is defined multiple times.',
                            )
                            return {"FINISHED"}

                        previous_object_names.append(object_name)
                        continue

                    mtllib_match = match(
                        "^\s*mtllib\s+([a-z_][a-z_0-9]*)\.mtl\s*$", line
                    )

                    if mtllib_match:
                        mtl_file = None

                        try:
                            mtl_file = open(
                                path.join(
                                    path.dirname(self.properties.filepath),
                                    mtllib_match[1] + ".mtl",
                                ),
                                "r",
                            )

                            material_name = None
                            waiting_for_texture = False

                            for mtl_line in mtl_file.readlines():
                                without_comments = match("^([^#]*)#.*$", mtl_line)

                                if without_comments:
                                    mtl_line = without_comments[1]

                                if match("\S", mtl_line):
                                    if match("^\s*Ns\s+.*$", mtl_line):
                                        continue

                                    if match("^\s*Ka\s+.*$", mtl_line):
                                        continue

                                    if match("^\s*Ks\s+.*$", mtl_line):
                                        continue

                                    if match("^\s*Ke\s+.*$", mtl_line):
                                        continue

                                    if match("^\s*Ni\s+.*$", mtl_line):
                                        continue

                                    if match("^\s*d\s+.*$", mtl_line):
                                        continue

                                    if match("^\s*illum\s+.*$", mtl_line):
                                        continue

                                    newmtl_match = match(
                                        "^\s*newmtl\s+([a-z_][a-z_0-9]*)\s*$", mtl_line
                                    )

                                    if newmtl_match:
                                        if waiting_for_texture:
                                            self.report(
                                                {"ERROR"},
                                                'Material "'
                                                + material_name
                                                + '" lacks a texture map.',
                                            )
                                            return {"FINISHED"}
                                        elif newmtl_match[1] in materials:
                                            self.report(
                                                {"ERROR"},
                                                'Material "'
                                                + material_name
                                                + '" is defined multiple times.',
                                            )
                                            return {"FINISHED"}
                                        else:
                                            material_name = newmtl_match[1]
                                            waiting_for_texture = True
                                            continue

                                    map_kd_match = match(
                                        "^\s*map_Kd\s+(.*)\.tga\s*$", mtl_line
                                    )

                                    if map_kd_match:
                                        if waiting_for_texture:
                                            waiting_for_texture = False

                                            materials[material_name] = create_material(
                                                material_name,
                                                path.join(
                                                    path.dirname(
                                                        self.properties.filepath
                                                    ),
                                                    path.dirname(mtllib_match[1]),
                                                    map_kd_match[1] + ".tga",
                                                ),
                                            )

                                            continue
                                        elif material_name == None:
                                            self.report(
                                                {"ERROR"},
                                                "Cannot specify a texture map outside of a material.",
                                            )
                                            return {"FINISHED"}
                                        else:
                                            self.report(
                                                {"ERROR"},
                                                "Cannot specify multiple texture maps for the same material.",
                                            )
                                            return {"FINISHED"}

                                    self.report(
                                        {"ERROR"},
                                        'Failed to parse "'
                                        + mtl_line
                                        + '" as a MTL instruction.',
                                    )
                                    return {"FINISHED"}

                            if waiting_for_texture:
                                self.report(
                                    {"ERROR"},
                                    'Material "'
                                    + material_name
                                    + '" lacks a texture map.',
                                )
                                return {"FINISHED"}

                        finally:
                            if mtl_file != None:
                                mtl_file.close()

                        continue

                    usemtl_match = match("^\s*usemtl\s+([a-z_][a-z_0-9]*)\s*$", line)

                    if usemtl_match:
                        if usemtl_match[1] in materials:
                            current_material = materials[usemtl_match[1]]
                            continue
                        else:
                            self.report(
                                {"ERROR"},
                                'Undefined material "' + usemtl_match[1] + '".',
                            )
                            return {"FINISHED"}

                    v_match = match(
                        "^\s*v\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))(?:\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))(?:\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+)))?)?\s*$",
                        line,
                    )

                    if v_match:
                        vertices.append(
                            [
                                float(v_match[1]),
                                float(v_match[2]),
                                float(v_match[3]),
                                float(v_match[4]) if v_match[4] is not None else 21.26,
                                float(v_match[5]) if v_match[5] is not None else 71.52,
                                float(v_match[6]) if v_match[6] is not None else 7.22,
                                float(v_match[7]) if v_match[7] is not None else 1,
                            ]
                        )
                        continue

                    vn_match = match("^\s*vn\s+.*$", line)

                    if vn_match:
                        continue

                    vt_match = match(
                        "^\s*vt\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))\s+(-?(?:\d+\.\d+|\d+\.|\d+|\.\d+))\s*$",
                        line,
                    )

                    if vt_match:
                        texture_coordinates.append(
                            [
                                float(vt_match[1]),
                                float(vt_match[2]),
                            ]
                        )
                        continue

                    s_match = match("^\s*s\s+.*$", line)

                    if s_match:
                        continue

                    f_match = match("^\s*f((?:\s+-?\d+\/-?\d+(?:\/\d+)?)+)\s*$", line)

                    if f_match:
                        # TODO: not true for navigation meshes!
                        if current_material == None:
                            self.report(
                                {"ERROR"},
                                "All faces must have materials.",
                            )
                            return {"FINISHED"}
                        elif object_name == None:
                            self.report(
                                {"ERROR"},
                                "Face outside of object.",
                            )
                            return {"FINISHED"}
                        else:
                            extracted = []

                            for fragment in f_match[1].strip().split():
                                indices = fragment.split("/")

                                if len(indices) < 2:
                                    # TODO: not true for navigation meshes!
                                    self.report(
                                        {"ERROR"},
                                        "All vertices of all faces must have texture coordinates.",
                                    )
                                    return {"FINISHED"}
                                else:
                                    vertex = int(indices[0])

                                    if vertex == 0:
                                        self.report(
                                            {"ERROR"},
                                            "Vertex indices cannot be zero.",
                                        )
                                        return {"FINISHED"}
                                    elif vertex > 0 and vertex <= len(vertices):
                                        vertex = vertex - 1
                                    elif vertex < 0 and vertex >= -len(vertices):
                                        vertex = len(vertices) + vertex
                                    else:
                                        self.report(
                                            {"ERROR"},
                                            "Out-of-range vertex index.",
                                        )
                                        return {"FINISHED"}

                                    texture_coordinate = int(indices[1])

                                    if texture_coordinate == 0:
                                        self.report(
                                            {"ERROR"},
                                            "Texture coordinate indices cannot be zero.",
                                        )
                                        return {"FINISHED"}
                                    elif (
                                        texture_coordinate > 0
                                        and texture_coordinate
                                        <= len(texture_coordinates)
                                    ):
                                        texture_coordinate = texture_coordinate - 1
                                    elif (
                                        texture_coordinate < 0
                                        and texture_coordinate
                                        >= -len(texture_coordinates)
                                    ):
                                        texture_coordinate = (
                                            len(texture_coordinates)
                                            + texture_coordinate
                                        )
                                    else:
                                        self.report(
                                            {"ERROR"},
                                            "Out-of-range texture coordinate index.",
                                        )
                                        return {"FINISHED"}

                                    extracted.append(
                                        [vertex, texture_coordinate, current_material]
                                    )

                            faces.append(extracted)
                            continue

                    self.report(
                        {"ERROR"},
                        'Failed to parse "' + line + '" as an OBJ instruction.',
                    )
                    return {"FINISHED"}

            end_object()

        finally:
            if file != None:
                file.close()

        return {"FINISHED"}


def menu_import(self, context):
    self.layout.operator(IndifferenceEngineOBJImport.bl_idname)


classes = (
    IndifferenceEngineOBJAddMaterial,
    IndifferenceEngineOBJAddNavigationMesh,
    IndifferenceEngineOBJAdd,
    IndifferenceEngineOBJExport,
    IndifferenceEngineOBJImport,
)


def register():
    for cls in classes:
        register_class(cls)

    VIEW3D_MT_add.append(menu_add)
    TOPBAR_MT_file_import.append(menu_import)
    TOPBAR_MT_file_export.append(menu_export)


def unregister():
    for cls in classes:
        unregister_class(cls)

    VIEW3D_MT_add.remove(menu_add)
    TOPBAR_MT_file_import.remove(menu_import)
    TOPBAR_MT_file_export.remove(menu_export)


if __name__ == "__main__":
    register()

# TODO: Configure render settings (resolution, antialiasing)
# TODO: Connect alpha
