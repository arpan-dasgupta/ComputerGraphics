import math
import numpy as np

vertices = [
    "0.5f,  0.1f,   0.0f,   ",
    "0.4045084971874737f,   0.1f,   0.29389262614623657f,   ",
    "0.15450849718747373f,  0.1f,   0.47552825814757677f,   ",
    "-0.15450849718747367f, 0.1f,   0.4755282581475768f,    ",
    "-0.40450849718747367f, 0.1f,   0.2938926261462366f,    ",
    "-0.5f, 0.1f,   0.0f, ",
    "-0.4045084971874738f,  0.1f,   -0.2938926261462365f,   ",
    "-0.15450849718747378f, 0.1f,   -0.47552825814757677f,  ",
    "0.15450849718747361f,  0.1f,   -0.4755282581475768f,   ",
    "0.40450849718747367f,  0.1f,   -0.2938926261462367f,   ",
    "0.5f,  -0.1f,  0.0f,   ",
    "0.4045084971874737f,   -0.1f,  0.29389262614623657f,   ",
    "0.15450849718747373f,  -0.1f,  0.47552825814757677f,   ",
    "-0.15450849718747367f, -0.1f,  0.4755282581475768f,    ",
    "-0.40450849718747367f, -0.1f,  0.2938926261462366f,    ",
    "-0.5f, -0.1f,  0.0f, ",
    "-0.4045084971874738f,  -0.1f,  -0.2938926261462365f,   ",
    "-0.15450849718747378f, -0.1f,  -0.47552825814757677f,  ",
    "0.15450849718747361f,  -0.1f,  -0.4755282581475768f,   ",
    "0.40450849718747367f,  -0.1f,  -0.2938926261462367f,   ",
]

shape_sides = [[0,1,2,3,4,5,6,7,8,9],[10,11,12,13,14,15,16,17,18,19],[1,0,10,11],[2,1,11,12],[3,2,12,13],
[4,3,13,14],[5,4,14,15],[15,16,6,5],[7,6,16,17],[8,7,17,18],[9,8,18,19],[0,9,19,10]]

# shape_sides = [[0,1,2]]

def generate():
    print("#ifndef __decagonalPrism_h__\n#define __decagonalPrism_h__")
    print("extern float vertices[] = {")
    for side in shape_sides:
        color = str(np.random.random()) + "f, " + str(np.random.random()) + "f, " + str(np.random.random()) + "f, "
        n = len(side)
        if n==3:
            print(vertices[side[0]]+color)
            print(vertices[side[1]]+color)
            print(vertices[side[2]]+color)
            print()
        else:
            for i in range(n-2):
                print(vertices[side[i]]+color)
                print(vertices[side[i+1]]+color)
                print(vertices[side[n-1]]+color)
                print()
        print()
    print("};\n#endif")

if __name__=="__main__":
    
    # For poly 1
    # theta = math.pi/5
    # rad = 0.5
    # height = 0.4
    # for i in range(10):
    #     print('"'+str(rad*math.cos(i*theta))+"f,\t" +str(height/2)+"f,\t" + str(rad*math.sin(i*theta))+"f,\t"+'",')
    # for i in range(10):
    #     print('"'+str(rad*math.cos(i*theta))+"f,\t" +str(-height/2)+"f,\t" + str(rad*math.sin(i*theta))+"f,\t"+'",')

    generate()