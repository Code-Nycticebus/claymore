import requests  # type: ignore
import zipfile
import os

# Download binaries

r = requests.get(
    "https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip"
)
with open("glew.zip", "wb") as f:
    f.write(r.content)
with zipfile.ZipFile("glew.zip", "r") as zip:
    zip.extractall()

os.rename("glew-2.2.0", "glew-win")
os.remove("glew.zip")
