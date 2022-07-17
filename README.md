<img src="https://github.com/DmitriySalnikov/godot_qoi/blob/5bd25a2dc2ea907041b4c9a7f4ae12bc0ae19a94/icon.png" width=128/>

# godot_qoi

This is a QOI ([Quite OK Image Format](https://github.com/phoboslab/qoi)) wrapper for Godot Engine. This addon will allow you to read, write, encode and decode images to or from the QOI format.

This is a GDNative library. Includes precompiled binaries for `Windows`, `Linux`, `macOS` and `Android`, but it must compile for each platform which Godot Engine supports.

## Features

* Fastest encoding
* Fast decoding
* Editor integration
* Cross-platform
* Simple API

## Disadvantage

* Large file size (compared to PNG or WebP. more than 2-3 times larger)
* No VRAM compression

## Approximate comparison of QOI encoding speed vs PNG

https://user-images.githubusercontent.com/7782218/152955097-285f81dc-2b65-4f80-bd6a-890b7280d806.mp4

## Support

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/I2I53VZ2D)

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://paypal.me/dmitriysalnikov)

[<img src="https://jobs.qiwi.com/assets/media/logo-mobile.83a2407e.svg" alt="qiwi" width=90px/>](https://qiwi.com/n/DMITRIYSALNIKOV)

## Download

To download, use the [Godot Asset Library](https://godotengine.org/asset-library/asset/1226) or download the archive by clicking the button at the top of the main repository page: `Code -> Download ZIP`, then unzip it to your project folder.

## Editor integration

* After installation, do not forget to enable the plugin in the project settings.

As with regular textures, `qoi` can be used as sprites or textures of materials. QOI will be imported into the `.import` folder to write additional settings to the original `.qoi` file.

Import parameters are also available:

![godot windows opt tools 64_nPWBco0itA](https://user-images.githubusercontent.com/7782218/178936308-6dbb0d6a-c742-4635-87ea-f46c6fa44645.png)

And you can save any texture as a `qoi` image directly in the editor:

![godot windows opt tools 64_xrBag12kP4](https://user-images.githubusercontent.com/7782218/178931905-00a45d4e-a331-4ccd-8a45-551831786db0.png)

### How to remove the editor integration?

If you do not need integration into the editor, you can simply delete or not extract the `addons/qoi/editor/` folder. You will still be able to work with images using scripts, but the editor will not recognize qoi.

## API

```gdscript
# Enable or disable error printing
# type bool, default true
print_errors

# Read the QOI image from file
# @return null or image
Image read(path : String)

# Decode the QOI image from an encoded byte array
# @return null or image
Image decode(data : PoolByteArray)

# Write the Image as a file in QOI format
# @return code of error
int write(path : String, img : Image)

# Encode the Image in QOI format as an array of bytes
# @return an array of bytes. If an error occurs, it is empty.
PoolByteArray encode(img : Image)
```

### Example

```gdscript
func example():
	var qoi = load("res://addons/qoi/qoi.gdns").new()
	qoi.print_errors = true
	
	qoi.write("user://example.qoi", load("res://icon.png").get_data())
	var img = qoi.read("user://example.qoi")
	var enc = qoi.encode(img)
	var dec = qoi.decode(enc)
	
	var tex: = ImageTexture.new()
	tex.create_from_image(dec)
	$TextureRect.texture = tex
```

## Benchmarks

To get these logs, the scene "testsuite/TestScene.tscn" was used.

Perhaps someone will need this data...

```txt
--- Based on default '2D' preset. Only compression mode changed ---

--- Avg for 5 runs, with 1920x1080 99 frames

-- Android, Honor 9X China, Kirin 810, Mali-G52 (MP6)

webp lossy:	26698.691 ms
webp:		28159.032 ms
etc2:		2823.541 ms
png:		29960.048 ms
qoi:		10672.911 ms

-- Windows 11, i7 3770, GTX 970

webp lossy:	1665.573 ms
webp:		2071.083 ms
s3tc:		192.595 ms
png:		4656.091 ms
qoi:		1939.103 ms
```

<details>
<summary>Detailed logs</summary>

```txt
--- Based on default '2D' preset. Only compression mode changed ---

-- Android, Honor 9X China, Kirin 810, Mali-G52 (MP6)

VRAM Compression:

Run: 0, Ext: png, 2931.171 ms
Run: 1, Ext: png, 2760.742 ms
Run: 2, Ext: png, 2815.537 ms
Run: 3, Ext: png, 2811.050 ms
Run: 4, Ext: png, 2799.204 ms
Run: 0, Ext: qoi, 10739.582 ms
Run: 1, Ext: qoi, 10636.819 ms
Run: 2, Ext: qoi, 10613.298 ms
Run: 3, Ext: qoi, 10688.168 ms
Run: 4, Ext: qoi, 10738.884 ms
Note 'importer_defaults/texture[compress/mode]' is not equal to 0. PNG was imported as VRAM compressed texture inside .import folder
Platform: Android
Avg for 5 runs, with 1920x1080 99 frames
etc2:        2823.541 ms
qoi: 10683.350 ms

WebP Lossy:

Run: 0, Ext: png, 26653.431 ms
Run: 1, Ext: png, 26598.585 ms
Run: 2, Ext: png, 26718.401 ms
Run: 3, Ext: png, 26750.491 ms
Run: 4, Ext: png, 26772.548 ms
Run: 0, Ext: qoi, 10593.349 ms
Run: 1, Ext: qoi, 10656.931 ms
Run: 2, Ext: qoi, 10685.101 ms
Run: 3, Ext: qoi, 10625.928 ms
Run: 4, Ext: qoi, 10657.248 ms
Note 'rendering/misc/lossless_compression/force_png' is off or 'importer_defaults/texture[compress/mode]' is lossy. PNG was imported as WebP inside .import folderPlatform: Android
Avg for 5 runs, with 1920x1080 99 frames
webp lossy:  26698.691 ms
qoi: 10643.711 ms

WebP Lossless:

Run: 0, Ext: png, 28225.728 ms
Run: 1, Ext: png, 28185.932 ms
Run: 2, Ext: png, 28140.142 ms
Run: 3, Ext: png, 28097.408 ms
Run: 4, Ext: png, 28145.951 ms
Run: 0, Ext: qoi, 10669.436 ms
Run: 1, Ext: qoi, 10664.980 ms
Run: 2, Ext: qoi, 10683.432 ms
Run: 3, Ext: qoi, 10702.113 ms
Run: 4, Ext: qoi, 10638.751 ms
Note 'rendering/misc/lossless_compression/force_png' is off or 'importer_defaults/texture[compress/mode]' is lossy. PNG was imported as WebP inside .import folderPlatform: Android
Avg for 5 runs, with 1920x1080 99 frames
webp:        28159.032 ms
qoi: 10671.742 ms

PNG:

Run: 0, Ext: png, 30080.978 ms
Run: 1, Ext: png, 29911.152 ms
Run: 2, Ext: png, 29905.833 ms
Run: 3, Ext: png, 29961.291 ms
Run: 4, Ext: png, 29940.987 ms
Run: 0, Ext: qoi, 10733.422 ms
Run: 1, Ext: qoi, 10697.980 ms
Run: 2, Ext: qoi, 10656.909 ms
Run: 3, Ext: qoi, 10710.381 ms
Run: 4, Ext: qoi, 10665.515 ms
Platform: Android
Avg for 5 runs, with 1920x1080 99 frames
png: 29960.048 ms
qoi: 10692.841 ms

-- Windows 11, i7 3770, GTX 970

VRAM Compression:

Run: 0, Ext: png, 273.765 ms
Run: 1, Ext: png, 176.404 ms
Run: 2, Ext: png, 170.142 ms
Run: 3, Ext: png, 172.205 ms
Run: 4, Ext: png, 170.459 ms
Run: 0, Ext: qoi, 1966.354 ms
Run: 1, Ext: qoi, 1963.467 ms
Run: 2, Ext: qoi, 1962.730 ms
Run: 3, Ext: qoi, 1973.714 ms
Run: 4, Ext: qoi, 1943.954 ms
Note 'importer_defaults/texture[compress/mode]' is not equal to 0. PNG was imported as VRAM compressed texture inside .import folder
Platform: Windows
Avg for 5 runs, with 1920x1080 99 frames
s3tc:   192.595 ms
qoi:    1962.044 ms

WebP Lossy:

Run: 0, Ext: png, 1702.099 ms
Run: 1, Ext: png, 1660.662 ms
Run: 2, Ext: png, 1653.947 ms
Run: 3, Ext: png, 1633.476 ms
Run: 4, Ext: png, 1677.683 ms
Run: 0, Ext: qoi, 1891.697 ms
Run: 1, Ext: qoi, 1910.959 ms
Run: 2, Ext: qoi, 1891.640 ms
Run: 3, Ext: qoi, 1889.121 ms
Run: 4, Ext: qoi, 1934.738 ms
Note 'rendering/misc/lossless_compression/force_png' is off or 'importer_defaults/texture[compress/mode]' is lossy. PNG was imported as WebP inside .import folder
Platform: Windows
Avg for 5 runs, with 1920x1080 99 frames
webp lossy:     1665.573 ms
qoi:    1903.631 ms

WebP Lossless:

Run: 0, Ext: png, 2166.656 ms
Run: 1, Ext: png, 2164.790 ms
Run: 2, Ext: png, 2022.571 ms
Run: 3, Ext: png, 1983.932 ms
Run: 4, Ext: png, 2017.466 ms
Run: 0, Ext: qoi, 1989.682 ms
Run: 1, Ext: qoi, 1894.387 ms
Run: 2, Ext: qoi, 1915.186 ms
Run: 3, Ext: qoi, 1922.443 ms
Run: 4, Ext: qoi, 1913.443 ms
Note 'rendering/misc/lossless_compression/force_png' is off or 'importer_defaults/texture[compress/mode]' is lossy. PNG was imported as WebP inside .import folder
Platform: Windows
Avg for 5 runs, with 1920x1080 99 frames
webp:   2071.083 ms
qoi:    1927.028 ms

PNG:

Run: 0, Ext: png, 4688.020 ms
Run: 1, Ext: png, 4595.850 ms
Run: 2, Ext: png, 4634.695 ms
Run: 3, Ext: png, 4661.570 ms
Run: 4, Ext: png, 4700.321 ms
Run: 0, Ext: qoi, 1972.156 ms
Run: 1, Ext: qoi, 1949.307 ms
Run: 2, Ext: qoi, 1976.162 ms
Run: 3, Ext: qoi, 1954.153 ms
Run: 4, Ext: qoi, 1966.760 ms
Platform: Windows
Avg for 5 runs, with 1920x1080 99 frames
png:    4656.091 ms
qoi:    1963.708 ms
```

</details>

## License

MIT license
