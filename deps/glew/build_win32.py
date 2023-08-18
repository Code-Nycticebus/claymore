import requests
import zipfile
r = requests.get("https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip")
print(r.status_code)
with open("glew.zip", "wb") as f:
    f.write(r.content)
with zipfile.ZipFile('glew.zip', 'r') as zip:
    zip.extractall()
import os
os.rename("glew-2.2.0", "glew-win")
os.remove("glew.zip")
