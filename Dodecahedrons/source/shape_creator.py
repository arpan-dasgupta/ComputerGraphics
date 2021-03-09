import math
import numpy as np

vertices = [
    "0.5f,  0.15f,   0.0f,   ",
    "0.4045084971874737f,   0.15f,   0.29389262614623657f,   ",
    "0.15450849718747373f,  0.15f,   0.47552825814757677f,   ",
    "-0.15450849718747367f, 0.15f,   0.4755282581475768f,    ",
    "-0.40450849718747367f, 0.15f,   0.2938926261462366f,    ",
    "-0.5f, 0.15f,   0.0f, ",
    "-0.4045084971874738f,  0.15f,   -0.2938926261462365f,   ",
    "-0.15450849718747378f, 0.15f,   -0.47552825814757677f,  ",
    "0.15450849718747361f,  0.15f,   -0.4755282581475768f,   ",
    "0.40450849718747367f,  0.15f,   -0.2938926261462367f,   ",
    "0.5f,  -0.15f,  0.0f,   ",
    "0.4045084971874737f,   -0.15f,  0.29389262614623657f,   ",
    "0.15450849718747373f,  -0.15f,  0.47552825814757677f,   ",
    "-0.15450849718747367f, -0.15f,  0.4755282581475768f,    ",
    "-0.40450849718747367f, -0.15f,  0.2938926261462366f,    ",
    "-0.5f, -0.15f,  0.0f, ",
    "-0.4045084971874738f,  -0.15f,  -0.2938926261462365f,   ",
    "-0.15450849718747378f, -0.15f,  -0.47552825814757677f,  ",
    "0.15450849718747361f,  -0.15f,  -0.4755282581475768f,   ",
    "0.40450849718747367f,  -0.15f,  -0.2938926261462367f,   ",
]

shape_sides = [[0,1,2,3,4,5,6,7,8,9],[10,11,12,13,14,15,16,17,18,19],[1,0,10,11],[2,1,11,12],[3,2,12,13],
[4,3,13,14],[5,4,14,15],[15,16,6,5],[7,6,16,17],[8,7,17,18],[9,8,18,19],[0,9,19,10]]

def generate1():
    count=0
    print("#ifndef __decagonalPrism_h__\n#define __decagonalPrism_h__")
    print("extern float vertices[] = {")
    for side in shape_sides:
        color = str(np.random.random()) + "f, " + str(np.random.random()) + "f, " + str(np.random.random()) + "f, "
        n = len(side)
        if n==3:
            print(vertices[side[0]]+color)
            print(vertices[side[1]]+color)
            print(vertices[side[2]]+color)
            count+=1
            print()
        else:
            for i in range(n-2):
                print(vertices[side[i]]+color)
                print(vertices[side[i+1]]+color)
                print(vertices[side[n-1]]+color)
                count+=1
                print()
        print()
    print("};\nextern int triangleCount = "+ str(count)+";\n#endif")

def generate4():
    count=0
    print("#ifndef __decagonalPrism_h__\n#define __decagonalPrism_h__")
    print("extern float vertices[] = {")
    for side in shape_sides:
        color1 = "0.0f,  0.0f, "
        color2 = "1.0f,  0.0f, "
        color3 = "0.0f,  1.0f, "
        n = len(side)
        if n==3:
            print(vertices[side[0]]+color1)
            print(vertices[side[1]]+color2)
            print(vertices[side[2]]+color3)
            count+=1
            print()
        else:
            for i in range(n-2):
                print(vertices[side[i]]+color1)
                print(vertices[side[i+1]]+color2)
                print(vertices[side[n-1]]+color3)
                count+=1
                print()
        print()
    print("};\nextern int triangleCount = "+ str(count)+";\n#endif")

# vertices = [
#     "0.5000000000f, 0.5000000000f, 0.5000000000f, ",
#     "-0.5000000000f, 0.5000000000f, 0.5000000000f, ",
#     "0.5000000000f, -0.5000000000f, 0.5000000000f, ",
#     "-0.5000000000f, -0.5000000000f, 0.5000000000f, ",
#     "0.5000000000f, 0.5000000000f, -0.5000000000f, ",
#     "-0.5000000000f, 0.5000000000f, -0.5000000000f, ",
#     "0.5000000000f, -0.5000000000f, -0.5000000000f, ",
#     "-0.5000000000f, -0.5000000000f, -0.5000000000f, ",
#     "0.0000000000f, 0.8090170026f, 0.3090170026f, ",
#     "0.0000000000f, -0.8090170026f, 0.3090170026f, ",
#     "0.0000000000f, 0.8090170026f, -0.3090170026f, ",
#     "0.0000000000f, -0.8090170026f, -0.3090170026f, ",
#     "0.3090170026f, 0.0000000000f, 0.8090170026f, ",
#     "-0.3090170026f, 0.0000000000f, 0.8090170026f, ",
#     "0.3090170026f, 0.0000000000f, -0.8090170026f, ",
#     "-0.3090170026f, 0.0000000000f, -0.8090170026f, ",
#     "0.8090170026f, 0.3090170026f, 0.0000000000f, ",
#     "-0.8090170026f, 0.3090170026f, 0.0000000000f, ",
#     "0.8090170026f, -0.3090170026f, 0.0000000000f, ",
#     "-0.8090170026f, -0.3090170026f, 0.0000000000f, ",
# ]

# shape_sides = [[9,2,12,13,3],[8,0,12,13,1],[11,6,14,15,7],[10,4,14,15,5],[16,0,8,10,4],[17,1,8,10,5],
#     [18,2,9,11,6],[12,2,18,16,0],[14,6,18,16,4],[19,3,9,11,7],[13,1,17,19,3],[15,5,17,19,7]]

def generate2():
    count=0
    print("#ifndef __dodecahedron_h__\n#define __dodecahedron_h__")
    print("extern float vertices[] = {")
    for side in shape_sides:
        color = str(np.random.random()) + "f, " + str(np.random.random()) + "f, " + str(np.random.random()) + "f, "
        n = len(side)
        if n==3:
            print(vertices[side[0]]+color)
            print(vertices[side[1]]+color)
            print(vertices[side[2]]+color)
            count+=1
            print()
        else:
            for i in range(n-2):
                print(vertices[side[i]]+color)
                print(vertices[side[i+1]]+color)
                print(vertices[side[n-1]]+color)
                count+=1
                print()
        print()
    print("};\nextern int triangleCount = "+ str(count)+";\n#endif")

# vertices = [
#     "0.0f, 0.5393446666666667f, -0.3333333333333333f, ",
#     "0.0f, 0.5393446666666667f, 0.3333333333333333f, ",
#     "0.0f, -0.5393446666666667f, -0.3333333333333333f, ",
#     "0.0f, -0.5393446666666667f, 0.3333333333333333f, ",
#     "0.5393446666666667f, -0.3333333333333333f, 0.0f, ",
#     "0.5393446666666667f, 0.3333333333333333f, 0.0f, ",
#     "-0.5393446666666667f, -0.3333333333333333f, 0.0f, ",
#     "-0.5393446666666667f, 0.3333333333333333f, 0.0f, ",
#     "-0.3333333333333333f, 0.0f, 0.5393446666666667f, ",
#     "0.3333333333333333f, 0.0f, 0.5393446666666667f, ",
#     "-0.3333333333333333f, 0.0f, -0.5393446666666667f, ",
#     "0.3333333333333333f, 0.0f, -0.5393446666666667f, ",
# ]

# shape_sides = [[1,7,10,11,5],[0,7,8,9,5],[3,4,11,10,6],[2,4,9,8,6],[3,2,11,5,9],[0,1,9,4,11],[2,3,8,7,10],
#     [6,8,1,0,10],[7,6,3,9,1],[3,4,5,1,8],[2,6,7,0,11],[2,4,5,0,10]]

def generate3():
    count=0
    print("#ifndef __greatDodecahedron_h__\n#define __greatDodecahedron_h__")
    print("extern float vertices[] = {")
    for side in shape_sides:
        color = str(np.random.random()) + "f, " + str(np.random.random()) + "f, " + str(np.random.random()) + "f, "
        n = len(side)
        if n==3:
            print(vertices[side[0]]+color)
            print(vertices[side[1]]+color)
            print(vertices[side[2]]+color)
            count+=1
            print()
        else:
            for i in range(n-2):
                print(vertices[side[i]]+color)
                print(vertices[side[i+1]]+color)
                print(vertices[side[n-1]]+color)
                count+=1
                print()
        print()
    print("};\nextern int triangleCount = "+ str(count)+";\n#endif")

if __name__=="__main__":
    
    # For poly 1
    # theta = math.pi/5
    # rad = 0.5
    # height = 0.4
    # for i in range(10):
    #     print('"'+str(rad*math.cos(i*theta))+"f,\t" +str(height/2)+"f,\t" + str(rad*math.sin(i*theta))+"f,\t"+'",')
    # for i in range(10):
    #     print('"'+str(rad*math.cos(i*theta))+"f,\t" +str(-height/2)+"f,\t" + str(rad*math.sin(i*theta))+"f,\t"+'",')

    # generate1()
    # generate2()
    # generate3()
    generate4()