bl_info = {
    "name": "Custom mesh format",
    "description": "Custom mesh format",
    "author": "Pavlo Lavrenenko",
    "version": (0, 2),
    "blender": (2, 6, 0),
    "location": "File > Import-Export",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}


import bpy
import struct


def write_mesh_data(context, filepath,
                    export_normals,
                    export_uvs,
                    export_tangents
                    ):
    if context.active_object.type != 'MESH':
        return {'CANCELLED'}

    mesh = context.active_object.to_mesh(context.scene, True, 'PREVIEW')

    if len(mesh.uv_textures) == 0:
        return {'CANCELLED'}

    vertices = mesh.vertices[:]
    faces = mesh.tessfaces[:]
    uv_texture = mesh.tessface_uv_textures.active.data[:]

    # TODO calculate tangents

    last_attribute = len(vertices)
    uv_coordinates = last_attribute * [None]
    for i, face in enumerate(faces):
        for j in range(3):
            current_coord = (uv_texture[i].uv[j][0], uv_texture[i].uv[j][1])
            if uv_coordinates[face.vertices[j]] != current_coord:
                if uv_coordinates[face.vertices[j]] == None:
                    uv_coordinates[face.vertices[j]] = current_coord
                else:
                    uv_coordinates.append(current_coord)
                    vertices.append(vertices[face.vertices[j]])
                    face.vertices[j] = last_attribute
                    last_attribute += 1

    magic = "MESH"
    vc_offset = 28  # Header length
    vn_offset = vc_offset + last_attribute * 12
    uv_offset = vn_offset + last_attribute * 12
    fc_offset = uv_offset + last_attribute * 8
    ti_offset = fc_offset + len(faces) * 12

    f = open(filepath, 'wb')

    f.write(struct.pack("<4s4b", magic.encode("utf-8"), 1, 0, 0, 0))

    # TODO write material parameters

    f.write(struct.pack("<5i", vc_offset, vn_offset, uv_offset,
                        fc_offset, ti_offset))

    for vertex in vertices:
        f.write(struct.pack("<3f", vertex.co[0], vertex.co[1], vertex.co[2]))

    for vertex in vertices:
        f.write(struct.pack("<3f", vertex.normal[0], vertex.normal[1], vertex.normal[2]))

    for uv_coordinate in uv_coordinates:
        f.write(struct.pack("<2f", uv_coordinate[0], uv_coordinate[1]))

    for face in faces:
        f.write(struct.pack("<3i", face.vertices[0], face.vertices[1], face.vertices[2]))

    f.close()

    return {'FINISHED'}


from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty


class ExportMesh(bpy.types.Operator, ExportHelper):
    '''Export mesh.'''
    bl_idname = "export.mesh"
    bl_label = "Export Mesh"

    filename_ext = ".mesh"

    filter_glob = StringProperty(
            default="*.mesh",
            options={'HIDDEN'},
            )

    export_normals = BoolProperty(
            name="Vertex normals",
            description="Export vertex normals",
            default=True,
            )

    export_uvs = BoolProperty(
            name="UV coordinates",
            description="Export UV coordinates",
            default=True,
            )

    export_tangents = BoolProperty(
            name="Vertex tangents",
            description="Export vertex tangents",
            default=True,
            )

    @classmethod
    def poll(cls, context):
        return context.active_object != None

    def execute(self, context):
        return write_mesh_data(context, self.filepath,
                               self.export_normals,
                               self.export_uvs,
                               self.export_tangents
                               )


def menu_func_export(self, context):
    self.layout.operator(ExportMesh.bl_idname, text="Mesh (.mesh)")


def register():
    bpy.utils.register_class(ExportMesh)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportMesh)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()
