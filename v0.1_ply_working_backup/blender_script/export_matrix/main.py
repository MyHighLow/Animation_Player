# This script exports the animation data of the selected object as a list of world matrices
import bpy
import mathutils
import copy
import json

def generate_scale_matrix(scale_vector):
	sx = mathutils.Matrix.Scale(scale_vector.x, 4, (1.0, 0.0, 0.0))
	sy = mathutils.Matrix.Scale(scale_vector.y, 4, (0.0, 1.0, 0.0))
	sz = mathutils.Matrix.Scale(scale_vector.z, 4, (0.0, 0.0, 1.0))
	return sx * sy * sz

# user-specified constants
OUTPUT_FILE_PATH = "D:/out.json"

# matrix is stored as (frame number, matrix)
matrices = []

# go to first frame
bpy.context.scene.frame_set(bpy.context.scene.frame_start)

# iterate through every frame
while (bpy.context.scene.frame_current <= bpy.context.scene.frame_end):
	matrix_world = bpy.context.active_object.matrix_world.transposed()
		
	if len(matrices) == 0 or matrices[-1]['transform'] != matrix_world:
		matrices.append({'frame' : bpy.context.scene.frame_current, 'transform' : tuple([tuple(i) for i in matrix_world])})
	bpy.context.scene.frame_set(1 + bpy.context.scene.frame_current)

# convert data to json data
json_data = {}
for i in matrices:
	json_data[i['frame']] = i['transform']

# write json data to file
json.dump(json_data, open(OUTPUT_FILE_PATH, 'w'))