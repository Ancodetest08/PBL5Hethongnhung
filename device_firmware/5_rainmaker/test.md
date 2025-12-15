Thực hiện Update Firmware

1. Terminal 1: erase-flash, build, flash, monitor

2. Sửa VERSION file CMakeLists.txt

3. Terminal 2: build lại

4. Vào ESP RainMaker Dashboard
    -> Firmware Images
    -> Add Image
    -> Image Name: Tên
       Type: Lightbulb
       Select image (Chọn file 5_rainmaker.bin ở build)
    -> Add Image
    -> Start OTA
    -> Job Name: Tên
       Nodes (Chọn node đang hoạt động)
    -> Start OTA