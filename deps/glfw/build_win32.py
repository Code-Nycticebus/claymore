import requests  # type: ignore
import zipfile
import os

# Download binaries

r = requests.get(
    "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN32.zip"
)
print(r.status_code)
with open("glfw.zip", "wb") as f:
    f.write(r.content)
with zipfile.ZipFile("glfw.zip", "r") as zip:
    zip.extractall()

os.rename("glfw-3.3.8.bin.WIN32", "glfw-win")
os.remove("glfw.zip")
