#
# Copyright (c) 2024-present Henri Michelon
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT
#
import bpy
import json
import os
import re
import sys
import math
import mathutils
import subprocess

bl_info = {
    "name": "Lysa Engine",
    "description": "Scene edition & export for the Lysa engine",
    "author": "Henri Michelon",
    "doc_url": "https://github.com/HenriMichelon/lysa",
    "tracker_url" : "https://github.com/HenriMichelon/lysa/issues",
    "version": (1, 0, 0),
    "blender": (4, 4, 0),
    "category": "Game Engine",
}

#----------------------------------------------------------------------------------------------------------------------
RESOURCES_ID = "resources"
NODES_TYPE   = {
    "EMPTY" : "node",
    "MESH"  : "mesh"
}
APP_URI      = "app://"
EXPORT_EXT   = ".glb"
Z_EXPORT_EXT = ".assets"
GLTF2LYSA = "gltf2lysa.exe"

# Drop down list value of Lysa built-in classes for nodes properties
NODES_CLASSES = [
    ("Camera", "Camera", "Camera"),
    ("Character", "Character", "Character"),
    ("CollisionArea", "CollisionArea", "CollisionArea"),
    ("DirectionalLight", "DirectionalLight", "DirectionalLight"),
    ("Environment", "Environment", "Environment"),
    ("KinematicBody", "KinematicBody", "KinematicBody"),
    ("MeshInstance", "MeshInstance", "MeshInstance"),
    ("Node", "Node", "Node"),
    ("OmniLight", "OmniLight", "OmniLight"),
    ("PhysicsBody", "PhysicsBody", "PhysicsBody"),
    ("RayCast", "RayCast", "RayCast"),
    ("RigidBody", "RigidBody", "RigidBody"),
    ("Skybox", "Skybox", "Skybox"),
    ("SpotLight", "SpotLight", "SpotLight"),
    ("StaticBody", "StaticBody", "StaticBody"),
]

# Drop down list value of BCn compression formats for scene export
COMPRESSION_FORMATS = [
    ("bc1", "BC1 (DXT1)", "bc1"), 
    ("bc2", "BC2 (DXT3)", "bc2"),
    ("bc3", "BC3 (DXT5)", "bc3"),
    ("bc7", "BC7", "bc7") 
  ]
  
EXPORT_OPTIONS = [
    ("scene", "Scene", "scene"),
    ("resources", "Resources", "resources"),
]

RESOURCES_OPTIONS = [
    ("export", "Export", "export"),
    ("link", "Link", "link"),
]


#----------------------------------------------------------------------------------------------------------------------

# converts a vec into a string
def convert_vector(vec):
    return str(vec.x) + "," + str(vec.z) + "," + str(-vec.y)

# converts a vec into a string
def convert_scale(vec):
    return str(vec.x) + "," + str(vec.z) + "," + str(vec.y)

# converts a quaternion into a string
def convert_quat(quat):
    return str(quat.x) + "," + str(quat.z) + "," + str(-quat.y) + "," + str(quat.w)

def show_message(message="", title="Error", icon='ERROR'):
    def draw(self):
        self.layout.label(text=message)
    bpy.context.window_manager.popup_menu(draw, title=title, icon=icon)


#----------------------------------------------------------------------------------------------------------------------

# adds a resource or a mesh node to the JSON scene file
def add_resource(nodes, obj, parent):
    if parent is None:
        path=obj.name
    else:
        path=parent + "/" + obj.name
    if obj.type == "MESH":
        nodes.append({
            "id": obj.name + ".mesh",
            "type" : NODES_TYPE[obj.type],
            "resource" : RESOURCES_ID,
            "path": path
        })
    for child in obj.children:
        add_resource(nodes, child, path)

# adds a node to the JSON scene file
def add_node(obj):
    #print(obj.name + ":" + obj.type)
    settings = bpy.context.scene.lysa_settings
    node = {"id": obj.name, "properties": {}}
    if "lysa_props" in obj:
        props = obj.lysa_props
        if "custom_class_name" in props and props.custom_class_name != "":
            node["class"] = props.custom_class_name
        else:
            if "class_name" in props:
                if props.class_name != "Node":
                    node["class"] = props.class_name
        if "properties" in props:
            custom_props = {}
            for custom_prop in props.properties:
                custom_props[custom_prop.name] = custom_prop.value.replace("$$", obj.name)
            node["properties"] = custom_props
    node["properties"]["position"] = convert_vector(obj.matrix_local.to_translation());
    if obj.rotation_mode == "QUATERNION":
        rot = obj.rotation_quaternion;
    else:
        rot = obj.rotation_euler.to_quaternion()
    node["properties"]["rotation"] = convert_quat(rot);
    node["properties"]["scale"] = convert_scale(obj.scale);
    if obj.hide_get():
        node["properties"]["visible"] = "false"
    if obj.type == "MESH":
        if settings.reconcile_mesh:
            mesh_name = re.sub(r"\.\d+$", "", obj.name)
        else:
            mesh_name = obj.name
        node["child"] = { "id": mesh_name + ".mesh" }
        node["child"]["duplicate"] = "true"
        if obj.children:
            node["child"]["children"] = [add_node(child) for child in obj.children]
    elif obj.children:
        node["children"] = [add_node(child) for child in obj.children]
    if obj.type == "LIGHT":
        original_rotation = obj.rotation_euler
        conversion_matrix = mathutils.Matrix.Rotation(math.radians(-90), 3, 'X')
        new_rotation = original_rotation.to_matrix() @ conversion_matrix
        node["properties"] = {
            "color" : str(obj.data.color.r) + "," + str(obj.data.color.g) + "," + str(obj.data.color.b) + "," + str(obj.data.energy/10.0),
            "position" : convert_vector(obj.location),
            "rotation" : convert_quat(new_rotation.to_quaternion())
        }
        if obj.data.use_shadow:
            node["properties"]["cast_shadows"] = "true"
        if obj.data.type == "POINT":
            node["class"] = "OmniLight"
            if obj.data.use_custom_distance:
                node["properties"]["range"] = str(obj.data.cutoff_distance)
        if obj.data.type == "SUN":
            node["class"] = "DirectionalLight"
        if obj.data.type == "SPOT":
            node["class"] = "SpotLight"
            node["properties"]["fov"] = str(obj.data.spot_size)
    return node

# Exports the Blender scene to a JSON scene
def export_json():
    settings = bpy.context.scene.lysa_settings
    directory = settings.models_directory
    filename = os.path.splitext(os.path.basename(bpy.data.filepath))[0]
    if settings.resources == "export":
        directory = settings.models_directory
        filename = os.path.splitext(os.path.basename(bpy.data.filepath))[0]
        res_file = APP_URI + directory + "/" + filename.replace("\\", "/") + ".json"
    else:
        res_file = APP_URI + settings.link_file
    result = {
        "includes": [
            res_file
        ]
    }
    nodes = []
    for obj in bpy.context.scene.objects:
        if obj.parent is None:
            nodes.append(add_node(obj))
    scene = { "id": filename }
    if nodes:
        scene["children"] = nodes
    result["nodes"] = nodes
    return result

def export_resources_json():
    settings = bpy.context.scene.lysa_settings
    directory = settings.models_directory
    filename = os.path.splitext(os.path.basename(bpy.data.filepath))[0]
    if settings.convert_assets:
        ext = Z_EXPORT_EXT
    else:
        ext = EXPORT_EXT
    filename = APP_URI + directory + "/" + filename.replace("\\", "/") + ext
    nodes = [{
        "id" : RESOURCES_ID,
        "type" : "resource",
        "resource" : filename
    }]
    for obj in bpy.context.scene.objects:
        if obj.parent is None:
            add_resource(nodes, obj, obj.parent)
    return { "nodes" : nodes}


#----------------------------------------------------------------------------------------------------------------------

class ExportOperator(bpy.types.Operator):
    bl_idname = "object.lysa_export_operator"
    bl_label = "Export to Lysa"

    def execute(self, context):
        settings = bpy.context.scene.lysa_settings
        blend_file_path = bpy.data.filepath
        #print("file: " + blend_file_path)
        if blend_file_path == "":
            show_message("Please save the blender project first")
            return {'CANCELLED'}
        if settings.project_directory == "":
            show_message("Please set the project directory in the scene properties first")
            return {'CANCELLED'}
        if not os.path.exists(settings.project_directory):
            show_message("Incorrect project directory " + settings.project_directory)
            return {'CANCELLED'}
        if settings.scene_directory == "":
            show_message("Please set the project scene directory in the scene properties first")
            return {'CANCELLED'}
        if not os.path.exists(settings.project_directory + "/" + settings.scene_directory):
            show_message("Incorrect scene directory " + settings.scene_directory)
            return {'CANCELLED'}
        if settings.models_directory == "":
            show_message("Please set the project models directory in the scene properties first")
            return {'CANCELLED'}
        if not os.path.exists(settings.project_directory + "/" + settings.models_directory):
            show_message("Incorrect model directory " + settings.models_directory)
            return {'CANCELLED'}
        blend_file_name = os.path.basename(blend_file_path)
        file_name = os.path.splitext(blend_file_name)[0];
        export_file_name = file_name + EXPORT_EXT
        export_resources_file_name = file_name + ".json"
        scene_dir = settings.scene_directory.replace("/", "\\")
        models_dir = settings.models_directory.replace("/", "\\")
        export_models_path = os.path.join(settings.project_directory, models_dir)
        export_scene_path = os.path.join(settings.project_directory, scene_dir)
        glb_export_path = os.path.join(export_models_path, export_file_name)
        resource_desc_export_path = os.path.join(export_models_path, export_resources_file_name)
        json_scene_export_path = os.path.join(export_scene_path, file_name + ".json")

        print("--------------------------------------------")
        bpy.context.window.cursor_set("WAIT")
        #self.report({'INFO'}, "Saving " + blend_file_name);
        bpy.ops.wm.save_mainfile()

        if settings.export == "scene":
            result = export_json()
            with open(json_scene_export_path, 'w') as json_file:
                json.dump(result, json_file, indent=2)

        if settings.export == "resources" or settings.resources == "export":
            if settings.resources_desc or settings.export == "scene":
                result = export_resources_json()
                with open(resource_desc_export_path, 'w') as json_file:
                    json.dump(result, json_file, indent=2)
            bpy.ops.export_scene.gltf(
                filepath=glb_export_path, 
                export_format='GLB',
                export_jpeg_quality=98,
                export_image_quality=98,
                export_tangents=True,
                export_cameras=False,
                export_unused_images=False,
                export_unused_textures=False,
                export_yup=True,
                export_animations=True,
                export_anim_slide_to_zero=True
                )
        else:
            convert = False
            
        if settings.convert_assets and not settings.resources == "link":
            zres_export_path = os.path.join(export_models_path,  file_name + Z_EXPORT_EXT)
            bpy.context.window.cursor_set("WAIT")
            subprocess.run([
                GLTF2LYSA,
                "-v",
                "-t", str(settings.gltf2lysa_threads),
                "-f", settings.gltf2lysa_format,
                glb_export_path, 
                zres_export_path])
            os.remove(glb_export_path)

        self.report({'INFO'}, "Project exported to Lysa")
        bpy.context.window.cursor_set("DEFAULT")
        return {'FINISHED'}


#----------------------------------------------------------------------------------------------------------------------

class CustomPropertyProperties(bpy.types.PropertyGroup):
    name: bpy.props.StringProperty(
        name="Name",
        default=""
    )
    value: bpy.props.StringProperty(
        name="Value",
        default=""
    )

def class_changed(self, context):
    obj = context.object
    if self.class_name == "StaticBody" or self.class_name == "RigidBody":
        prop = obj.lysa_props.properties.add()
        prop.name = "shape"
        prop.value = "MeshShape"
        prop = obj.lysa_props.properties.add()
        prop.name = "layer"
        prop.value = "1"
    
class CustomProperties(bpy.types.PropertyGroup):
    class_name: bpy.props.EnumProperty(
        name="Class",
        description="The Node's clas",
        items=NODES_CLASSES,
        default="Node",
        update=class_changed
    )
    custom_class_name: bpy.props.StringProperty(
        name="Custom class",
        description="The Node's class",
        default="",
    )
    properties: bpy.props.CollectionProperty(type=CustomPropertyProperties)

# Operator to add a custom property
class OBJECT_OT_AddCustomProperty(bpy.types.Operator):
    bl_label = "Add Custom Property"
    bl_idname = "object.lysa_add_custom_property"

    def execute(self, context):
        obj = context.object
        prop = obj.lysa_props.properties.add()
        prop.name = ""
        prop.value = ""
        return {'FINISHED'}

# Operator to remove a custom property
class OBJECT_OT_RemoveCustomProperty(bpy.types.Operator):
    bl_label = "Remove Custom Property"
    bl_idname = "object.lysa_remove_custom_property"
    index: bpy.props.IntProperty()

    def execute(self, context):
        obj = context.object
        obj.lysa_props.properties.remove(self.index)
        return {'FINISHED'}


class ObjectPanel(bpy.types.Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "Lysa Node"
    bl_idname = "OBJECT_PT_custom"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"

    def draw(self, context):
        layout = self.layout
        object = context.object
        props = object.lysa_props
        layout.prop(props, "class_name")
        layout.prop(props, "custom_class_name")

        index = 0
        for prop in object.lysa_props.properties:
            row = layout.row()
            row.prop(prop, "name")
            row.prop(prop, "value")
            row.operator("object.lysa_remove_custom_property", text="Remove").index = index
            index = index + 1
        layout.operator("object.lysa_add_custom_property")



#----------------------------------------------------------------------------------------------------------------------

class CustomSettings(bpy.types.PropertyGroup):
    project_directory: bpy.props.StringProperty(
        name="Project",
        description="The game project directory",
        default=""
    )
    scene_directory: bpy.props.StringProperty(
        name="Scene",
        description="The scene resource directory, relative to the project directory",
        default="res/scenes"
    )
    models_directory: bpy.props.StringProperty(
        name="Models",
        description="The models resource directory, relative to the project directory",
        default="res/models"
    )
    export: bpy.props.EnumProperty(
        name="Export as",
        description="Format to export to",
        items=EXPORT_OPTIONS,
        default="scene"
    )
    resources: bpy.props.EnumProperty(
        name="Resources",
        description="Export or link resources to scene",
        items=RESOURCES_OPTIONS,
        default="export"
    )
    resources_desc: bpy.props.BoolProperty(
        name="Export resources description",
        description="Also export JSON resources description",
        default=True
    )
    convert_assets: bpy.props.BoolProperty(
        name="Convert to Lysa assets pack",
        description="Convert the exported GLB file to a Lysa assets pack file then delete the GLB",
        default=True
    )
    link_file: bpy.props.StringProperty(
        name="Resource file",
        description="Link the scene to this resource file, relative to the project directory",
    )
    gltf2lysa_threads: bpy.props.IntProperty(
        name="Threads",
        description="Number of threads for gltf2lysa executable (0 = auto)",
        default=0,
        min=0,
        max=20
    )
    gltf2lysa_format: bpy.props.EnumProperty(
        name="Format",
        description="Compression format for color textures",
        items=COMPRESSION_FORMATS,
        default="bc7"
    )
    reconcile_mesh: bpy.props.BoolProperty(
        name="Reconcile and duplicate meshes",
        description="Try to reconcile meshes names between scene and resources by removing versions in meshes names",
        default=True
    )

class ScenePanel(bpy.types.Panel):
    """Creates a Panel in the Scene properties window"""
    bl_label = "Lysa project"
    bl_idname = "SCENE_PT_custom"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "scene"

    def draw(self, context):
        layout = self.layout
        scene = context.scene
        settings = scene.lysa_settings
        layout.prop(settings, "project_directory")
        layout.prop(settings, "scene_directory")
        layout.prop(settings, "models_directory")
        layout.prop(settings, "export")
        #layout.prop(settings, "resources")
        if settings.export == "scene":
            layout.prop(settings, "resources")
            if settings.resources == "link":
                layout.prop(settings, "link_file")
                layout.prop(settings, "reconcile_mesh")
            else:
                layout.prop(settings, "convert_assets")
        else:
            layout.prop(settings, "resources_desc")
            layout.prop(settings, "convert_assets")
        if settings.convert_assets and (settings.export == "resources" or settings.resources == "export"):
            layout.prop(settings, "gltf2lysa_format")
            layout.prop(settings, "gltf2lysa_threads")
        layout.operator("object.lysa_export_operator")


#----------------------------------------------------------------------------------------------------------------------

def add_keymap():
    wm = bpy.context.window_manager
    km = wm.keyconfigs.active.keymaps.get("3D View")
    # Define a new keymap item
    if km:
        kmi = km.keymap_items.new(ExportOperator.bl_idname, 'E', 'PRESS', ctrl=True)
        kmi.active = True

def register():
    bpy.utils.register_class(ExportOperator)
    bpy.utils.register_class(CustomSettings)
    bpy.utils.register_class(CustomPropertyProperties)
    bpy.utils.register_class(CustomProperties)
    bpy.types.Object.lysa_props = bpy.props.PointerProperty(type=CustomProperties)
    bpy.types.Scene.lysa_settings = bpy.props.PointerProperty(type=CustomSettings)
    bpy.utils.register_class(ScenePanel)
    bpy.utils.register_class(ObjectPanel)
    bpy.utils.register_class(OBJECT_OT_AddCustomProperty)
    bpy.utils.register_class(OBJECT_OT_RemoveCustomProperty)
    add_keymap()

def unregister():
    bpy.utils.unregister_class(OBJECT_OT_AddCustomProperty)
    bpy.utils.unregister_class(OBJECT_OT_RemoveCustomProperty)
    bpy.utils.unregister_class(ObjectPanel)
    bpy.utils.unregister_class(ScenePanel)
    bpy.utils.unregister_class(CustomSettings)
    bpy.utils.unregister_class(CustomProperties)
    bpy.utils.unregister_class(CustomPropertyProperties)
    bpy.utils.unregister_class(ExportOperator)


#-------------------------------------------------------------------------------------
print("---------------------------")
if __name__ == "__main__":
    register()
#    result = export_json()
#    json.dump(result, sys.stdout, indent=4);
    

