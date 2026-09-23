cord = str(input("koordinat : "))

oneLineCord = cord.replace(" ", "")

cordList = cord.split(",")

floatCord = [float(x) for x in cordList]

length = len(floatCord)

print("")

scale = float(input("scale : "))

for i in range(length):
    floatCord[i] = floatCord[i] * scale

print("")

for i in range(0, length - 1, 3):
    print(f"{floatCord[i]}, {floatCord[i+1]}, {floatCord[i+2]},")
    
print(f"{floatCord[length-3]}, {floatCord[length-2]}, {floatCord[length-1]}")