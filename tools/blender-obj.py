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
import re
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


def initializeScene(context):
    context.scene.render.engine = "BLENDER_EEVEE"

    for window in context.window_manager.windows:
        for area in window.screen.areas:
            if area.type == "VIEW_3D":
                for space in area.spaces:
                    if space.type == "VIEW_3D":
                        space.shading.type = "MATERIAL"


class IndifferenceEngineOBJAddMaterial(Operator, ImportHelper):
    bl_idname = "indifference_engine_obj.add_material"
    bl_label = "Add Indifference Engine OBJ Material"
    filename_ext = ".tga"

    def execute(self, context):
        initializeScene(context)

        material = None

        for material_candidate in bpy.data.materials:
            if material_candidate.use_nodes:
                for node in material_candidate.node_tree.nodes:
                    if node.type == "TEX_IMAGE":
                        if (
                            node.image is not None
                            and node.image.filepath == self.properties.filepath
                        ):
                            material = material_candidate

        if material is None:
            name = path.basename(path.splitext(self.properties.filepath)[0])

            if not re.match("^[a-z_][a-z_0-9]*$", name):
                self.report(
                    {"ERROR"},
                    "Texture name file names should be lower case, start with a letter or digit, followed by only letters, digits and/or underscores.",
                )
                return {"FINISHED"}

            if bpy.data.materials.get(name):
                index = 2

                while bpy.data.materials.get(name + "_" + str(index)):
                    index += 1

                name = name + "_" + str(index)

            material = bpy.data.materials.new(name)

            material.use_nodes = True

            output = None

            for node in material.node_tree.nodes:
                if node.type == "OUTPUT_MATERIAL":
                    output = node
                else:
                    material.node_tree.nodes.remove(node)

            vertex_color = material.node_tree.nodes.new("ShaderNodeVertexColor")
            vertex_color.layer_name = "Attribute"
            vertex_color.location = (-500, -50)

            tex_image = material.node_tree.nodes.new("ShaderNodeTexImage")
            tex_image.image = bpy.data.images.get(
                self.properties.filepath
            ) or bpy.data.images.load(self.properties.filepath)
            tex_image.interpolation = "Closest"
            tex_image.location = (-600, -225)

            mix = material.node_tree.nodes.new("ShaderNodeMix")
            mix.data_type = "RGBA"
            mix.blend_type = "MULTIPLY"
            mix.inputs[0].default_value = 1
            mix.location = (-250, 0)

            emission = material.node_tree.nodes.new("ShaderNodeEmission")
            emission.location = (0, 0)

            output.location = (250, 0)

            material.node_tree.links.new(mix.inputs[6], vertex_color.outputs[0])
            material.node_tree.links.new(mix.inputs[7], tex_image.outputs[0])
            material.node_tree.links.new(emission.inputs[0], mix.outputs[2])
            material.node_tree.links.new(output.inputs[0], emission.outputs[0])

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
        initializeScene(context)

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
        initializeScene(context)

        # TODO

        return {"FINISHED"}


def menu_export(self, context):
    self.layout.operator(IndifferenceEngineOBJExport.bl_idname)


class IndifferenceEngineOBJImport(Operator, ImportHelper):
    bl_idname = "import.indifference_engine_obj"
    bl_label = "Import Indifference Engine OBJ (*.obj)"
    filename_ext = ".obj"

    def execute(self, context):
        initializeScene(context)

        # TODO

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
