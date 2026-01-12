import re

vertices = []
faces = []

with open("cat.obj") as f:
    for line in f:
        line = line.strip()
        if line.startswith("v "):
            # parse vertex
            nums = list(map(float, line[2:].split()))
            vertices.append(nums)
        elif line.startswith("f "):
            # parse face (ignore /texcoord/normal)
            indices = []
            for part in line[2:].split():
                m = re.match(r"(\d+)(/\d*)?(/\d*)?", part)
                if m:
                    idx = int(m.group(1)) - 1  # OBJ is 1-indexed
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

with open("cat-converted.obj", "w") as out:
    out.write("std::vector<Vec3> vs = {\n")
    for v in vertices:
        out.write(f"    {{{v[0]}, {v[1]}, {v[2]}}},\n")
    out.write("};\n\n")

    out.write("std::vector<std::vector<int>> fs = {\n")
    for f in faces:
        out.write("    {" + ", ".join(map(str, f)) + "},\n")
    out.write("};\n")

print("Conversion complete: cat-converted.obj")
