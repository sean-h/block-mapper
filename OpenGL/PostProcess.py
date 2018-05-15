import bpy

# Variables passed from BlockEditor
map_import_path = '#map_import_path'
map_export_path = map_import_path.replace('.dae', '.fbx')

# Delete default scene
bpy.ops.object.select_all(action='SELECT')
bpy.ops.object.delete()

# Import map
bpy.ops.wm.collada_import(filepath=map_import_path)

# Remove doubles
for obj in bpy.context.scene.objects:
    bpy.ops.object.select_all(action='DESELECT')
    bpy.context.scene.objects.active = obj
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.mesh.remove_doubles()
    bpy.ops.object.mode_set(mode='OBJECT')

# Set unit system
bpy.context.scene.unit_settings.system='METRIC'

# Export as fbx
bpy.ops.export_scene.fbx(filepath=map_export_path, check_existing=False, apply_unit_scale=True, global_scale=1, apply_scale_options='FBX_SCALE_UNITS')