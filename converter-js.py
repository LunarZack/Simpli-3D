import re

vertices = []
faces = []

with open("among-us-3D-model-cgian.obj") as f:
    for line in f:
        line = line.strip()
        if line.startswith("v "):
            nums = list(map(float, line[2:].split()))
            vertices.append(nums)
        elif line.startswith("f "):
            indices = []
            for part in line[2:].split():
                m = re.match(r"(\d+)(/\d*)?(/\d*)?", part)
                if m:
                    idx = int(m.group(1)) - 1
                    indices.append(idx)
            faces.append(indices)

if vertices:
    xs = [v[0] for v in vertices]
    ys = [v[1] for v in vertices]
    zs = [v[2] for v in vertices]

    x_center = (max(xs) + min(xs)) / 2
    y_center = (max(ys) + min(ys)) / 2
    z_center = (max(zs) + min(zs)) / 2

    vertices = [[x - x_center, y - y_center, z - z_center] for x, y, z in vertices]

with open("among-us-3D-model-cgian.obj-js.js", "w") as out:
    out.write("const vs = [\n")
    for v in vertices:
        out.write(f"    {{ x: {v[0]}, y: {v[1]}, z: {v[2]} }},\n")
    out.write("];\n\n")

    out.write("const fs = [\n")
    for f in faces:
        out.write("    [" + ", ".join(map(str, f)) + "],\n")
    out.write("];\n")

print("Conversion complete: among-us-3D-model-cgian.obj-js.js")
