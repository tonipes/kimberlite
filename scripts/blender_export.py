import bpy
import os
import sys
import json
import mathutils
import struct
import argparse

import logging as logger

logger.basicConfig(level=logger.INFO,
                    format='%(asctime)s - %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')

EXPORT_KWARGS = {
    "export_format":    "GLB",
    "use_selection":    True,
    "check_existing":   False,
    "export_extras":    True,
    "export_apply":     True,
    "export_texcoords": True, 
    "export_normals":   True,
    "export_colors":    True,
    "export_tangents":  True,
    "export_morph":     False,
}


####################
# Helper Functions #
####################


def exit_editmode():
    if bpy.ops.object.mode_set.poll():
        bpy.ops.object.mode_set(mode='OBJECT')


########################
# Main Export Function #
########################


def export_scene(filepath, **kwargs):
    exit_editmode()

    scene = bpy.context.scene
    
    # Uses hide_render tag to select what to export
    export_objs = [obj for obj in scene.objects if not obj.hide_render]

    for obj in export_objs:
        obj.select_set(True)
    
    bpy.ops.export_scene.gltf(filepath=filepath, **kwargs)


def parse_args():
    parser = argparse.ArgumentParser(description='Blender export script')

    parser.add_argument('-o', '--output', help="Output file")

    # Only use arguments after "--"
    argv = sys.argv
    i = argv.index("--") if "--" in argv else None
    argv = argv[argv.index("--") + 1:] if i else None

    return parser.parse_args(argv)

def main():
    if not bpy.data.filepath: raise Exception("Blend file is not saved")

    args = parse_args()
    
    os.makedirs(os.path.dirname(args.output), exist_ok=True)

    export_scene(args.output, **EXPORT_KWARGS)

main()
