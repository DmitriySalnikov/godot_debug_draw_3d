extends HBoxContainer

var _on_versions_loaded_callback = null
@onready var btn: OptionButton = $OptionButton

func _enter_tree():
	hide()


func _ready():
	if OS.has_feature('web'):
		_on_versions_loaded_callback = JavaScriptBridge.create_callback(_on_versions_loaded)
		var versions_callbacks: JavaScriptObject = JavaScriptBridge.get_interface("versions_callbacks")
		versions_callbacks.loaded = _on_versions_loaded_callback
		
		JavaScriptBridge.eval("loadVersions()")


func _on_versions_loaded(args: Array) -> void:
	if (args.size() == 0):
		return
	
	var current_version: String = args[0]
	
	var versions_str: String = JavaScriptBridge.eval("versions_callbacks.versions;")
	var version_urls_str: String = JavaScriptBridge.eval("versions_callbacks.version_urls;")
	var versions: PackedStringArray = versions_str.split(";", false)
	var version_urls: PackedStringArray = version_urls_str.split(";", false)
	
	if versions:
		show()
		btn.clear()
		btn.item_selected.connect(func(idx):
			# move to another version
			JavaScriptBridge.eval("window.location.href = \"%s\"" % version_urls[idx])
		)
		
		for i in range(versions.size()):
			btn.add_item(versions[i], i)
			
			if versions[i] == current_version:
				btn.select(i)
