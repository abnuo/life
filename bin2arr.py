fn = "luke.txt"
with open(fn,"r") as f:
  data = f.read()

array = "{"+",".join(list(data))+"};"
print(array)