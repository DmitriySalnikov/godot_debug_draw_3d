GDPC                 �                                                                      #   X   .godot/exported/133200997/export-3f6dfedcd5ab4046d211567bf6df0470-DebugDrawDemoScene.scn�q      xz      �k0+S2O�~��P9O    X   .godot/exported/133200997/export-9aea7a5946d3b0c2d9d1e1b0c1a72c60-VisualizerAudioBus.res �     T      �����|�>TL8%    T   .godot/exported/133200997/export-9c301d5b8a21b7b00cee58224ccb0626-headless_test.scn �           ����Xγ�q��e<�Q�    P   .godot/exported/133200997/export-e7de4a5fa3dad1fa22f4b34ade96ef28-addon_icon.scn�      �      ���rm�}P�휺�me       .godot/extension_list.cfg   �$     5       ?F�Q�T���j����    $   .godot/global_script_class_cache.cfg�            9>�ʱ�p�˛OL�7u    P   .godot/imported/PixelatedElegance.ttf-aac00801d681e5d2b42b23257b2692a7.fontdata      �      Vz}Y�/��T}�_�m�    L   .godot/imported/Roboto-Bold.ttf-3674de3d9ad3ee757cd4b4a89f1e126d.fontdata   �6     ɞ     @��hE�2]N�@7�ҧr    @   .godot/imported/icon.png-487276ed1e3a0c39cad0279d744ee560.ctex  ��            
·�0���O��gM��       .godot/uid_cache.bin�!     &      j�*m��΍��.5Uq    0   addons/debug_draw_3d/debug_draw_3d.gdextension          �      �"�� x�1N�'��       check-only_test.gd.remap@     *       Z�:V�[A�Y������       check-only_test.gdc ��     �      �a
5�Sݾ�,w�V�    ,   examples_dd3d/DebugDrawDemoScene.gd.remap        ;       �*3§=S2�L4 	��    $   examples_dd3d/DebugDrawDemoScene.gdc@*      HG      �2�ݹE
��kr    ,   examples_dd3d/DebugDrawDemoScene.tscn.remap P     o       :�VUy7K9�i%C    (   examples_dd3d/DebugDrawDemoSceneCS.tscn �      �      �3�e��#��y�J    ,   examples_dd3d/PixelatedElegance.ttf.import  �5     �       A��W�5c֪��R+f    $   examples_dd3d/Roboto-Bold.ttf.importp�     �       �M6xj�΅�]����    ,   examples_dd3d/VisualizerAudioBus.tres.remap �     o       ������C�}i��Lr    $   examples_dd3d/addon_icon.gd.remap   `      3       GT�$�f��^=��       examples_dd3d/addon_icon.gdc�      �      5��{P���ǜyj�    $   examples_dd3d/addon_icon.tscn.remap �      g       �En�0�k���%z/R�    ,   examples_dd3d/demo_camera_movement.gd.remap �     =       ��Q��Y�o�ˀ��    (   examples_dd3d/demo_camera_movement.gdc  ��      �      ���e]|kD���j    ,   examples_dd3d/demo_music_visualizer.gd.remap      >       ��?�C��4WiK&�Fm    (   examples_dd3d/demo_music_visualizer.gdc ��      �      �a�eh�/ѕ���چF    ,   examples_dd3d/demo_settings_panel.gd.remap  @     <       �������=�zNnba�    (   examples_dd3d/demo_settings_panel.gdc         '	      5Q'���w�Ƴ��    4   examples_dd3d/demo_web_docs_version_select.gd.remap �     E       �!o5�ie��$@&�V�    0   examples_dd3d/demo_web_docs_version_select.gdc  P     �      ר����^�Pb�$K�:       headless_test.tscn.remapp     j       �^��b��J�J%�       icon.png�     �      ��T"<vWA"��\�f��       icon.png.import ��     �       Lg�W-Ǭ��E��f-       project.binary  0%           �0�4�Lg��O�ڠ��    [configuration]

entry_symbol = "debug_draw_3d_library_init"
compatibility_minimum = "4.1.4"
reloadable = false

[dependencies]

; example.x86_64 = { "relative or absolute path to the dependency" : "the path relative to the exported project", }
; -------------------------------------
; debug

macos = { }
windows.x86_64 = { }
linux.x86_64 = { }

; by default godot is using threads
web.wasm32.nothreads = {}
web.wasm32 = {}

android.arm32 = { }
android.arm64 = { }
android.x86_32 = { }
android.x86_64 = { }

ios = {}

; -------------------------------------
; release no debug draw

macos.template_release = { }
windows.template_release.x86_64 = { }
linux.template_release.x86_64 = { }

web.template_release.wasm32.nothreads = { }
web.template_release.wasm32 = { }

android.template_release.arm32 = { }
android.template_release.arm64 = { }
android.template_release.x86_32 = { }
android.template_release.x86_64 = { }

ios.template_release = {}

; -------------------------------------
; release forced debug draw

macos.template_release.forced_dd3d = { }
windows.template_release.x86_64.forced_dd3d = { }
linux.template_release.x86_64.forced_dd3d = { }

web.template_release.wasm32.nothreads.forced_dd3d = { }
web.template_release.wasm32.forced_dd3d = { }

ios.template_release.forced_dd3d = {}

[libraries]

; -------------------------------------
; debug

macos = "libs/libdd3d.macos.editor.universal.framework"
windows.x86_64 = "libs/libdd3d.windows.editor.x86_64.dll"
linux.x86_64 = "libs/libdd3d.linux.editor.x86_64.so"

web.wasm32.nothreads = "libs/libdd3d.web.template_debug.wasm32.wasm"
web.wasm32 = "libs/libdd3d.web.template_debug.wasm32.threads.wasm"

android.arm32 = "libs/libdd3d.android.template_debug.arm32.so"
android.arm64 = "libs/libdd3d.android.template_debug.arm64.so"
android.x86_32 = "libs/libdd3d.android.template_debug.x86_32.so"
android.x86_64 = "libs/libdd3d.android.template_debug.x86_64.so"

ios = "libs/libdd3d.ios.template_debug.universal.dylib"

; -------------------------------------
; release no debug draw

macos.template_release = "libs/libdd3d.macos.template_release.universal.framework"
windows.template_release.x86_64 = "libs/libdd3d.windows.template_release.x86_64.dll"
linux.template_release.x86_64 = "libs/libdd3d.linux.template_release.x86_64.so"

web.template_release.wasm32.nothreads = "libs/libdd3d.web.template_release.wasm32.wasm"
web.template_release.wasm32 = "libs/libdd3d.web.template_release.wasm32.threads.wasm"

android.template_release.arm32 = "libs/libdd3d.android.template_release.arm32.so"
android.template_release.arm64 = "libs/libdd3d.android.template_release.arm64.so"
android.template_release.x86_32 = "libs/libdd3d.android.template_release.x86_32.so"
android.template_release.x86_64 = "libs/libdd3d.android.template_release.x86_64.so"

ios.template_release = "libs/libdd3d.ios.template_release.universal.dylib"

; -------------------------------------
; release forced debug draw

macos.template_release.forced_dd3d = "libs/libdd3d.macos.template_release.universal.enabled.framework"
windows.template_release.x86_64.forced_dd3d = "libs/libdd3d.windows.template_release.x86_64.enabled.dll"
linux.template_release.x86_64.forced_dd3d = "libs/libdd3d.linux.template_release.x86_64.enabled.so"

web.template_release.wasm32.nothreads.forced_dd3d = "libs/libdd3d.web.template_release.wasm32.enabled.wasm"
web.template_release.wasm32.forced_dd3d = "libs/libdd3d.web.template_release.wasm32.threads.enabled.wasm"

ios.template_release.forced_dd3d = "libs/libdd3d.ios.template_release.universal.enabled.dylib"

; -------------------------------------
; DOUBLE PRECISION
; -------------------------------------

; -------------------------------------
; debug

macos.double = "libs/libdd3d.macos.editor.universal.double.framework"
windows.x86_64.double = "libs/libdd3d.windows.editor.x86_64.double.dll"
linux.x86_64.double = "libs/libdd3d.linux.editor.x86_64.double.so"

web.wasm32.nothreads.double = "libs/libdd3d.web.template_debug.wasm32.double.wasm"
web.wasm32.double = "libs/libdd3d.web.template_debug.wasm32.threads.double.wasm"

android.arm32.double = "libs/libdd3d.android.template_debug.arm32.double.so"
android.arm64.double = "libs/libdd3d.android.template_debug.arm64.double.so"
android.x86_32.double = "libs/libdd3d.android.template_debug.x86_32.double.so"
android.x86_64.double = "libs/libdd3d.android.template_debug.x86_64.double.so"

ios.double = "libs/libdd3d.ios.template_debug.universal.dylib"

; -------------------------------------
; release no debug draw

macos.template_release.double = "libs/libdd3d.macos.template_release.universal.double.framework"
windows.template_release.x86_64.double = "libs/libdd3d.windows.template_release.x86_64.double.dll"
linux.template_release.x86_64.double = "libs/libdd3d.linux.template_release.x86_64.double.so"

web.template_release.wasm32.nothreads.double = "libs/libdd3d.web.template_release.wasm32.double.wasm"
web.template_release.wasm32.double = "libs/libdd3d.web.template_release.wasm32.threads.double.wasm"

android.template_release.arm32.double = "libs/libdd3d.android.template_release.arm32.double.so"
android.template_release.arm64.double = "libs/libdd3d.android.template_release.arm64.double.so"
android.template_release.x86_32.double = "libs/libdd3d.android.template_release.x86_32.double.so"
android.template_release.x86_64.double = "libs/libdd3d.android.template_release.x86_64.double.so"

ios.template_release.double = "libs/libdd3d.ios.template_release.universal.double.dylib"

; -------------------------------------
; release forced debug draw

macos.template_release.forced_dd3d.double = "libs/libdd3d.macos.template_release.universal.enabled.double.framework"
windows.template_release.x86_64.forced_dd3d.double = "libs/libdd3d.windows.template_release.x86_64.enabled.double.dll"
linux.template_release.x86_64.forced_dd3d.double = "libs/libdd3d.linux.template_release.x86_64.enabled.double.so"

web.template_release.wasm32.nothreads.forced_dd3d.double = "libs/libdd3d.web.template_release.wasm32.enabled.double.wasm"
web.template_release.wasm32.forced_dd3d.double = "libs/libdd3d.web.template_release.wasm32.threads.enabled.double.wasm"

ios.template_release.forced_dd3d.double = "libs/libdd3d.ios.template_release.universal.enabled.double.dylib"
             GDSCd   �  (�/�`�} &�D@o�t�g*�֏ܻ���F����æ������j� \QE��{i0TEUED<���o�) 7 7 m�ß*q𐁄�����m����\�.��D���m���7��+HBN|DBSW�m_�]��2����������(X��~�_��!�1�Ӈcak{��]�E�3+��(9�4������L�����G�(����gB��#0@ �,�%�?���(�D��N��ˋ�E��F�e��`�F�����&i*/B��2֋�Kj��3�1��J���(�y@'P#
ZPR���v
�E&�f�a��L�#<d�,��2��8aJ�,�b(hA�r����6�+4'�V��2�]��}̖y�M�%�a:��4�>��n�����bԺ���JLwh�/�i�)�4Aի(��k�Čc��r#�k��2�BC�,����6PX��T�o���7�����8M=pL:-��h���dT0�����{��!��Y�y��>?�إ2�h���<�����qGjW/4.`�˶6�xO�� ���F糦z��ghs�May��X������a�E�ݓ���_���X��[u�n���~�ם����̯��y�WYj���;�mތ�`c'n���<�^���U�go�X����E}V��Qr�,��#{�c��������!<�
n=��ņ�� ��H55�g�           RSRC                    PackedScene            ��������                                            f      resource_local_to_scene    resource_name    background_mode    background_color    background_energy_multiplier    background_intensity    background_canvas_max_layer    background_camera_feed_id    sky    sky_custom_fov    sky_rotation    ambient_light_source    ambient_light_color    ambient_light_sky_contribution    ambient_light_energy    reflected_light_source    tonemap_mode    tonemap_exposure    tonemap_white    ssr_enabled    ssr_max_steps    ssr_fade_in    ssr_fade_out    ssr_depth_tolerance    ssao_enabled    ssao_radius    ssao_intensity    ssao_power    ssao_detail    ssao_horizon    ssao_sharpness    ssao_light_affect    ssao_ao_channel_affect    ssil_enabled    ssil_radius    ssil_intensity    ssil_sharpness    ssil_normal_rejection    sdfgi_enabled    sdfgi_use_occlusion    sdfgi_read_sky_light    sdfgi_bounce_feedback    sdfgi_cascades    sdfgi_min_cell_size    sdfgi_cascade0_distance    sdfgi_max_distance    sdfgi_y_scale    sdfgi_energy    sdfgi_normal_bias    sdfgi_probe_bias    glow_enabled    glow_levels/1    glow_levels/2    glow_levels/3    glow_levels/4    glow_levels/5    glow_levels/6    glow_levels/7    glow_normalized    glow_intensity    glow_strength 	   glow_mix    glow_bloom    glow_blend_mode    glow_hdr_threshold    glow_hdr_scale    glow_hdr_luminance_cap    glow_map_strength 	   glow_map    fog_enabled 	   fog_mode    fog_light_color    fog_light_energy    fog_sun_scatter    fog_density    fog_aerial_perspective    fog_sky_affect    fog_height    fog_height_density    fog_depth_curve    fog_depth_begin    fog_depth_end    volumetric_fog_enabled    volumetric_fog_density    volumetric_fog_albedo    volumetric_fog_emission    volumetric_fog_emission_energy    volumetric_fog_gi_inject    volumetric_fog_anisotropy    volumetric_fog_length    volumetric_fog_detail_spread    volumetric_fog_ambient_inject    volumetric_fog_sky_affect -   volumetric_fog_temporal_reprojection_enabled ,   volumetric_fog_temporal_reprojection_amount    adjustment_enabled    adjustment_brightness    adjustment_contrast    adjustment_saturation    adjustment_color_correction    script 	   _bundled       Script "   res://examples_dd3d/addon_icon.gd �"�Cؽ;   
   local://1 �	         local://PackedScene_st64x �	         Environment             d         PackedScene    e      	         names "         icon    script    Node3D    Camera 
   transform    environment    current 	   Camera3D    box    gizmo    grid    sphere 	   cylinder    line    1    2    	   variants                      �?              �?              �?        ض�@                   ��>���<~:��&Su��)�>&Su��.>_'�>ׄT>Ҿ`�>)	y>   �5?    �5�穖�~�h?穖���$?#�>��$?                 �?�������3      �?    ���3����  �?��׼%����׼   �|�>��T>��߾���B@�>�Xq��8>��>zr�>`��>Y4�>?M>   <�>�@m>�����#����>����G�<�>��2>H8h�b���	�g?     �?              �?              �?w�?Ů�H'?     �?              �?              �?�O�;�a�>A��=      node_count    
         nodes     \   ��������       ����                            ����                                       ����                        	   ����                       
   ����                           ����                           ����                           ����                     ����      	                    ����      
             conn_count              conns               node_paths              editable_instances              version       d      RSRC  GDSCd   H�  (�/�`H��9Z���T8
k��>l&��O�i/2���t��s��b9
wz�%؇�u.��Y���q��~�l,ˆ^xv��W^��5[z��Z��]��W�3���ǽ���y~-�5���]2�!x��w���3�,�ǋ��s��}?������L�W|��o�g���B��{f���)�<.��x��x�8?�G�x��x3����y���x�V�C���_ϓ�������� �y�_��@�K<�_���O�yX��x&<&�F<O�������<A�}���B<O��wlt[�����|��\�ۢ�����wX��Ί?�;t����"�?}g���o�����|ď�-�wZ�+��w(t!t�:|����8�������O�kz�/�k�n~*�]�S^o?�d~������'�6�Q^k��������~'�N~&�*� �%�'�)�
@G���o�됿�� ?	�?��vn� ���A���?�Y�{���	�~/f	�D��_��(pď�O�*(-�+/ )!�"����!LMZ�����A9M!�bbS���T�R����ܹ�<���%%,RMTP.HEQ91=᱄XD@Rċ&-@�`�x�
TNZ�`Z��41Qq��B��CӓTԕ!LORQ=E==5�"�x���#��h�	�KT��JK�*����[*��),TX��~ �&�8����"EG��d�Rkb"��GDP�j�Ai��	�ii��i�K���^����A�������TS�rrҥ�"&L�H�}�@9M)9=)9QA)5=e�EUt�$E�A�������"���,PJZ�������?��CQTM�$yB�ȉ&W~�����������%�.M*��K�"7��	T�RSӖ���"�&*�&���;###��!NHxj�)%�����QNT@C�����I5a�¥V������ȇ�(����~!!##����Ӕ�E��%(%�)(~ؓUTFFESTJTV����15I51A5U�gLPQKMV����RMLRQEX����D5qyR��wQui���iNV� #��-\��X(���r�Rdd4e�����Pui�*bj�A驉����(+=XF@R��ٱΣBQPM5QeQJ�bxb�I ";09I�0��)')K�P��Ji��2�I	#(�
*� B�%�R �h��KT�%c`YG!		IxH51A9=���Sa�I�I�+LPM:8q�_9������L�Ճ�S�TJRP~�����z�;w���;'^x��I"����/^�`;~�%�j�rJ�}���Pui��!�������������\O�c����w:Pui�zj�(�,ET]�b����%dp�\p�muK��
(���j�gɔ�6gL�u�����-!,-��~���Eg�l`�����������&dj���y�]��Ӆ�����녋ݹK�&�֛417���M��z��-#��L6Bot�Ԁ�h�������u�,4+kb�z��Aur��1�n��Gf��#�b4x���X�E�ʲ���]t����!-޸��y^�8@<a�gKVb������$��#��rcˍ�9n�ذ9����,��V��9��gL�4�9_��^Fh��!��Ym�iHԩ#���i�	ۄo�-B!-;9޼r<r��a��h'�������w������4��{:��w�ꠑ���N
<ϳ�fP9-1���i��L���:c�q獞���/F�洉�٪ÆhءcFm���AcȂ�l�a("6=>3�,px���m\���0_��(��M�Ζ�7�؜,�-:jh��	GG��?�9Z=��<�:Y��&Ҋ#4-8c�a���d�[oz�j���� -���Fn�L7Cu��ϗ0����h���b��nx����p�k�	�/0`�Λ���&����:`^s��]i܄��n��,�4b#µq�i����ym��4�K+ˍߕ�p``�ฆ��ɒm�	O F��ߡ�&��8`����$��y`��1�͛/�!�4q����a��7mt��B9YDX`i��� ��M6�9iڜ���<���������<�6<�k#���x�'��ymx�7�Oԅ�硡h�Do�v|�<o�V���de�����L8Yh�h���iҴ���/�	g��<7[��19s�vx���{j��<K����B��V�ɭ���9��e��<7�������F�<��_Ff�����<ϋ��y����Aip��I��B����<��y��yl��_���y�y��yk��'�a��ɼB�¢�uU2H�UC�v��▱8����Y�
�q2��'���ma�'c�+~�*����n�,���`P�4�ma��~L�-�`q]U���q]U��X\W�q[�cq]��m���uU��/G�O��8���ɪ|�ph�ɞ��OV���َ�=<`?YU�.����B��uUq[���J�hq]��m���4�v\W��>���𵸮�'t����'�{z���A�(�,5N0"�#���:s�h�J�C��7)���.\������@

(2����D��ؐk2R 
��3ff���f��L�S6̢̦̠09�##� ��"ج�j�!c�l��`a#�S2,V���DfD��,&��u�4��TQ���1*1$1�f�� �-9� C�%B�z	��A��!�b�2`��b
�!� AQ�!�(�GQO[_�ZX� %|�Z���� ����q�Zj�d Q@a�� ��@U���
,
�X��m���'�e����@
�?>?=u�}�v�r�`.���r�`.��`.���r�\.����\.���r�\.���r�\.��`0��s�b�,s�`.�悹\.���r�\.��`0��`0��r)Q�(��b0�K]����RUUUUUUUUUUUUUUUUU���q���E}�䉲|��j�-y"�K�N�鉌��� �}w�D��h?D�wJ~H�	���|g���n���?��] ~(�]�*�wH~7��; �߁�CV|��ݾ�������Y}W䇚����ݐ�]���]�����2$�]?�ݏ��]���;?D�?�;?��w9~( ��P��~(��f?��;���.�CD�	��l(������l��~~F�w>?K�w=?�;�����g_����
��μ��333�u��]1��k��Q�Ռ�Iൌ��ym�όxe�Ϧ^W�3^�~6�U?�򚂟Ay�gL^���	�t~V�����y��^�������y5��2x��3^��l�뛟�j�����z^ۼ��k�en��/S����x�/c�+��1^�x�	^K�25�j^��+	^f{Žl�k�˄xM�^Ѽl�U��ay�ˬ���eQ^ϼ�W3/C�
��y���6x���b�x�+>�u�z���.�z���j���z���y��c>�N�1�X�k>��*��fx��Ǩ�>�cH^�1�Q|,ǫ��x-�B(:�+/
q�j�B�W'^h�+^(����L�BM^żВ�%^ȃ�$^H�+/�z=�x^�� ;�_>���At^���x�����AW��AS�
�Ae^�� 0�@|�?|ԫ�bz��nx��Ax�ტ�r� (�St�*�AH^�A �n���W��������x^����,���偾xez�H�W���u�j ��9<�@Q�5<��+t� �����:���*�y�၀�Zy �T��u�u�x�Rv�k���x��0����Յ���e���0���aQ��x]�aV�L�� ^������è^i�zU�l�W��+	���*��ZxM�$�H�����f�!��~�x����q�k���W��8�E���^��O����c{��?c~vx��?T^w������u�??^e������W�����>a���'|�y�W��l��}(���>x��!����|�_�>A�m���������y�5�G��	��_�}6��x����/�_p�-�/:�
��ſ�/�*���?���k����O�_������������ˈ_ο�����?�I�Z���o�/�?�//�_�����u��Ż��׼+N���a����$NF:�Wˏʜb�w�U��اӞ[�ȑ#G���`�C�$~�c��c�v��4ǎ�����yl:����Ħ��}�vl*{�g-��ؐ!�=�M���Y�4���L�s_;i�B|�
��LdɸQ�*?~�П���J7U�د^��W�}���HR2�d�O�$f�n�g�Z���)�>i��n��������ʝ;v�ءq�TaI
�C�b��/�񆌠(��Y���*C�}�VYH"��d2�L&��T�7�1����_���?5�I����*s��2�ܧFMƀ�`�4��26�Wg�4��D�$<�g\���>4>4i҂U\�_�����&]�*�}�v����5~�,@��l�ok���]���\~L&ͦ�Ņ�o�}2��� �`
�̟9�HF�3�f�N�O�D!>�)��쓶�P��5~�I"����>i��n1��(�r&��;�N��9HG�3����I ��q�I�e�p�I�HZ�׼�Vnv5��y��p��g�Z��ͮ��2>��W|B���緟q�W!D2Z3��h�>/v��B��V��Qn����B"�$�����VR+��(�y�P��ϋ}~{�C�C��0�B�f�m6���^�5���;n�jl2d��5j��=�;n��mr��qk�l�{�5j��=�����-O��\l�?1R;�4%�x��2�|6�WP#ւG���[�O�J�`��f'҂J"���=��x$���#�>i+5�I�HX��\.���L$�hb�G%����B�6V�`��M,�h'	��w�N�v�/ �7���H��7e�	��v��}Ykh2x�3 ��w����N$0I����>i;�+��kqK$qP�-�'FG�	<�O�d�x49��J�����'m�F�E��ث,����WRl������N$�h>q4?�����+)6�gf���AW�~`����<�_I���쓶�CZ,��Ɂ+�+)6���}Ҥ��B^���%�G3WR�+>+��I �<��~%�椐HF�EE2�-0�^pa�Z�'m�؉$e'(�*�b�7��Ѽ]I��><�� �V�I�+)6���>i�!N`����% "��1�'�}�6H+�&��{N
_1鋢4_p��
�\ ҂U�_9`sQ쓶K�`�̟��y m;�4��L���:�b�jbsd�I���)��쓶���$x5��H��د����}�����`��'��b�_5��?&�I���)��쓶���5~�@Z����&6���>i�,�l�?1r	$�J���>iʿ0�Tbs�Ib��Ȇ��>i���Gl�4b���1��쳶ᲈ���m�60��m���W�[(���'ӆ�FE<�sr۪�F�N��dC�7�>�6̷p)��]�o��񴓶������c��i���^���~�>쓶�H��O��&{��&�-O��I._Ԅ�Q=퓶:�T�-_Ԅ�M�elR;kr��۴�{��8r�5��0�W���O�jܦe���hܦe�F�ӸM�>��4nӲO�=�q��}f�s_�i���Lܦe�F{.nӲO칸M�>��\ܦe�Y��<� �[�Y�%����Y�e�ſ�}�vI�n���Y[ف��!ᰃ7��	����i����*���5����.������_�n�a��.��k��I��'�H���a�����j�q@�����)���w��oc��6\`���7�7�A��}�S�ɰOچ[�Y*��#�G�nFj�q0�}"���Uv��e�x�'�q����S��ae�����珫���Ť�#�>��HFJ��E������}�VYxA� >�NV�2Mv�?&���6v\�ɿ%{�o2���Ba��1�w�>i{��0ߖG��yd?��g�i�fd�l�[��m����'�5#�L���+a�G���=��0?	�-ڌ�\~#�p�_ � �F����<�fvD�! �#�AX܎�-�`q;�Ѓ��m���E�a��qQ�m!��E�a��m�C�=���;e��w��}*
R�v�*�-��Ƅ	���~�;A=�H;T�N>!�'�6�'m��>1�@)3	T�� ���a���wQ��Z�w�-�';N�w�I�2����:Y2_���A�v\��u�U�$�ԉ'��ưi���vv1i�t�'m;�0[`ɀ��XU�	�.`���u�q[ѧc�$*I*fC��9d��(�0O!�i�"��8��:�P�阝a��1��tE�1�p��	�8B	%��J�-ڸ�(*Z���٣ ��=
��F������$<򕰛��"1��^cĮ��J$���㉬���`���/�"+?Eb*���� �"+?Eb*���F�]���"1��^Į��JEb���244�fS�E�b��xu�5ZĮ��~���IBj_�>���T#�C-b?3
f���|:��g�����BH���/�"�_���UCDo��/�Wv3����l��Yۯ姻anUk��D���Fi�u��֩Sgb7�=�d�v^���'D�d�Ie<-��G$т��I�a���I{*�w �ɧ�>i���T'KF����&`I+X�>Y�t�wQ��RB岟Ib�"�:�N�*B����
�NY�A:t��Q:�-I$�&���Rv&���8g��
�r��G�bM��`�qV��E8����h1Y����Î[�j��D���Î{E[�	�qB�*��D��J�㘈�P�v\(?����e�A5����'m��B~4�>���M0G���$����ְ��>���`�ɣ�z�J�_�#�<̑@>}�'Mf?j: ��W�E~XfͦYv\�M�M~XfͦM;�c����.M�}���PvQ�䘨K�1����sb�*�ڎ�9|����ΜYu�q�o��ى�-�v�U��`ۅPq�q���Po�qL�)&~Ov.?&s�Mk�q�����vv��e���T"^-?*s�g�
��.&�VG;w�}�d��=�oa"h�Jī�Ge电a�b?�Iku䴹��I�I�'�9ͦ4�C��B����}�\�3|*����i���K+SL���l�ǭ_��?��,�3�c�B����T*[���ߒ݌���N�H;��Q>a�EP�9l�nu���|�d��"~L%�����dN���w����i��"hD�6l��B��[�
[F��͵����Օ^�7y�
(����)�0���M�;�V���<�EL��2�,���/0H<�����2_V4�.�DZy�� �xEeNY�q2��I�-쬲�ka���NV�2Mv�?&���6v\(?���PR �|�d�4ƌ3f̘1cƌ#F���
���m�
�$���B}򽱛�_��c$�d�
V)Ҿj�}�ɍ�1��l�;�orI�)�k�H��݌T�|��	X"b���b�&y�0�̬�`&mWS&��-U����'�����Y�P��O��&�"վ��!��I!чHEܪ=�'��K�!� j{(O@�0��M�";�h;�%�-��������ŋ�!&ӆ�&�z�R]���p�~{�)\jr�'����4U�K@����II��{,�J����";��BU��$���Tm{�K��o�	T(����p��J'qPK�[�gm�Y"h�T"�VZB�,X⫪���}U"L�UU��#21�VI W:���25�A���������+a��25�:�ץ}�Vʀ�+�N%�ej s��K����H��$;�Ȗ��!�-����U$;��~��!ҐC���^���5���Yp��-�P�?@=�D�!~K4Ԥ�!(��!�-��&A X)Jd�Ŀ�}��~`�(�����Y�jd��V�A�Z8�%��~�Zd�4[�Dv�v�B�
*���N�)�)�Z�&��"EJM�`"-II5�C&��	���Y�Z4�������>k�����Y��5���DZ!q����Bv֖'n��'\B��[x�Nԉ:�C�M� 	�>,�i˰�Lti�5�h� !�A �����h4�%J�(Q�D�%J�@ j�j�r�_�T.�0��{u����U��MjTv"�P")IJ;,x����'�G D       � 0�9��aL��#4F!JDIFF��@& ��   k�V��=���u��6����t���~�G0�&v�c���b<����=ұɺfAt<s� ����\� ����~J-�e.U�Z�M"��G��g�a��gΘ	�X)a�����F�p�����RL$����Jǣ����G��3,�)�*��UfxnD���={���=�i9��"��Ҧ�QR����[���]e�0�ǜ�5c���%��E����g�|r�5�·x�X^��d�x*|Ip�M~���;�gq�L�'�㹸C����ļd�%4��%��gg>�mI��:{A���w%hY�#-���Ie?���Gi�
쌄��j2Ӂ��!q�q��G�ގ�ЅX��s�)�Q���������s������;r����G�T��.��͸/��������
�x���d����*�e���Y����,Kacno�\�.#W���~�[ۢK�0����Ʒ�ҽ-��<��u���?ٛ�t��>	�܉uw�}����{�:�Q�q��Y��ˢ>�ԑ_@,p���M6��T�-�ՏK_̐�]V��mx�vfH������fжuЎ6�������[��������t��5-�a�m׾%R_N/�۲����O��k�ێ)u�q �1����yA�����xj��Pqo�?�b����ɻ�����J�Y_�u���O�)p_�X�?���5c&����9������Ge��I�kӜ&�Sv��#jwԜ�����/xǐUҏ�zO�s�1������v�	�aJ��D��^�e��`�x�<B��Z���n�7�R"�6�����8�ø9��	�Ep#e+Əw�K�mF:�$�'�S�b����H�	bi��9�z�ԙ���^#�[AY7��؋3�B!����_?��-���+Z+�/�2�r�0 [�/���
�ܡõ�fwd���X:�ɑ'�6s�w�p2;��ġ�����l�l^7Ȝ�'⏟e���Y���il�ACfW�s�43-��+���_�`���̊���Ua(�D!<T��o���wJ��izB,.�=;�I�T��,x�v[[�u�5D�{�0�X�p|�]>�mӋ\�;��F����A�zŲ��������2G��:È�mb�Gq�]l@#z�g�_a��4uS5�|+Y��3%�~I�k��f���no0��7�t���%c��̊�2�^������-1^lA���t8y��`X(�E����#���<U{p�w�t8���ME���)�C�z襎Ĺ�8��vF��y܅Y�����a�7�9IlD��WL�@�^�9�(f�6�c�5?�#�����Q����֦D�P?J�o�o��!���ރ���)o�nJu`}F���j8�;��Bx�Ob��������k[7Ɓ�_�R��/�,w�{��.,u�~�m56�Xى�jGv����ss�1�4��մ��P���칩,� ]������Z$b��
��+��/����0ۤb=`Sd嶝\����D1���$ǤA%��m�Fn��[8s�i g�&�
K�YkyY�RcJ�NR�
��+�i�����Q7��^L�p�4��Νc�^2��b㏏6N�8s��t�Ӭ8KXI�P�>�����h��j���=D�(g�"i	{������3gw���{�)z��w_2@���el��l|��ev�_�!���q$�36CT[t�
� L��_�!|m�/Cf�c��'k�m�wd�aĊf���`���Jd6.���Y�q]o�cȀ�%�)M.��C1~��Ll|�����j��g�r�萇������%k+�%��,�L�g7k
nhN�yl�%m+�d�t1j�5On�ԓej����$d;HR�A$�3/k�,-��o]{�}��z�{ӥ���ވ\'(��iƗE�s�e%�r���r�Nꆐ�5&엘�]l�!��7�rok;��@
c���*ha�Y^�1���.����SM�<G��2ƶ�04�,Q�2H�0˦Ύ>2<��_�yA�WY7#���WP~�sQ��ݞ[����CK���uX��gc再Y׳��\ ��?�ݰj�U�V��c�y/CpS���kN%B�6wJ��v���!o��Sя9���G8����������v[�1���RW�5T�<Ї�Q���\��Bg��?x�3>�-��e���_�q\*��������l�Wv�y. �ƛ�� �|���Z�	)� �γ!�,�>#�7լ�	����v[���>��&?~t,'�-rX��>,�����G�^�:�/ԇpݘ[o��/�Bo����J>3��(�=N�=_'��ܡ��§`�$iC���iP~��HA� %s�L�!�M�鬼��Q����jL]������l�5��������hK���=8�⠷�om#����a��C���n�Yl6����ڵvjP�E#���gL��`�6�+c����M�n������(���m�<gt�{��l8��_y��Ƭǽ��/) 2�ui謌�U���.��OV�t�1qo��
>{������vn�8��
Dyg�b��ӻ[nFp_��>N�u}�@�U�rFz��}�Im	ۗ�\�����܈�=3�܀^$'`����*wh��#�q�9�/�2��;\�rfW迍��.k���ޢm��n;�Ӊ��-�ӝU���+2��`>��)���:[BpV>�ȁ�p˧������Ud�6���*u�,&p�Z���`���q<g��OX�@�F�{c�\�2��̖�����h�Ɔ|�Ol�d���&z/�_�m�@Y��V���2��=M���.�-��o���y��	�Ά�@�:��78�O/�qbjL��50b*fȀ��:�u���i�:�K��:������d�i���|yV� ��Ug�I���nc�Gn��^���i��9`g}
I����.�Ɯ��
{����b�BD�� ct�N�::%�h�Pe�v��F�K��=�,��cm�Xe�b�]����qy��q���a>�;m�3���TB��&�]B��{&׊x�����,�&�g�Y�D�jt�Z~3�y�;@Y��!�u��>�mj�S�%:��}�n�s�"&@_=��jl~)��7)}-��1XB���=lMHhk;��n��YLu4:ffɧ������7��3�y7ۥY�v�``���٬��m|Y5���)hX�޵�:�xh8���c>��N_��*��],ۑZy8t�6��2�B�w6v,�B���:77_��Bj5U�z��ee�l�L~���֚�G)�d�a���T�a��i���uV��a⯖�e�fL<�B�0<���Yz��~���&r���N-ٷ5,if>�Z|]a��ڈՄפb��ti�[���a�w_fc�/�����_�^o7i��=�᫦.�s���H�\�J�.�>�X�����&�㱷�� ��z�J�t�?�Ѕ->�z��u��t�����4k��BԾ;u[��>,\Ȇ5����v�9=|Lө�k��"���q�*��[x���n�\T���=��(a`��{ީ����.�o�g�Y4	��;X)S{�v���΀�6�t2���Q��A��D{i����)|��9�������Q��PB��h�+k
��-j<����h�6���-o=�M�p��x��8�zG���ňI�ZQ�؍gُ���G��i�#h���r4���>%��B��c��:`vme3�F�g�1�6����T�	�eu,�|jS�]/�!�f�k�m�io4Ot��8�	�2��\��%7���ƛ�[��%q���Q+愓��In5�"��v�s��f�֩�x ��.�g>fwB���n#~>إwD��.d��� �7}���J��Pف��_2�-�̍n�~�e�iG1���a����+��#d���g?�v7��v2��lsP2E���j�<��S��@��B\V����$ih�d
^� �N�>Gg�zt�u�hǧ5Y� ��r��pz�"G�Q}N��ޜ���������a�;80���Cw4��e�0�&G%!��՝����%V#6$[�*8�nA����C�14�ͭF� ��M�8�@���Y؇c�;_�ax�v�6�|�-�':ne�W����l���=|.>��f��Ra�������=���{��|t�ckw^w/`Z3g��n<�"�U��#�P�A�� .��;�x�?� ���N���˔L�d�of�!��x<Wb�[#�f�Y�Y�����"[{�u�������e<�f���������B�b�yc(��Ņ�#ל�)��M�Y<�s�i�����m��N��U]7�M�d�݄E�����\]�˟'�����m]�d)��h7��7X���}J�QSƛo࿑>3|c�B��B��{���L��������I.�����X<�n;.g�e'��K'�s��1ޒsp�yh��^6���-��y�Fݒ�V�,���HV]���a��E�j?�8M����!qw�eJ��e��f��M�L����l��$`��z��Q�c�%�.ͻ�T������&���1�obZ�6��b,.x]����r`�f�E�ޢ���)A�uavD8�FD�?�-sjA8�q����롁dG̅��d8�X_�����B��#��s覗�� }g���sl��y�O��g-O�-�0��g b�ڔ	��>���?�\��c���_��,�~�t��NO��NG���֞���c�-���s#�xSs-���+{�������!&Q�x�ʱ@L�g�q��_QY��z�w����Ób ~�jHK�c����!Lt��v���L �0̝G�=#��̾����s�?&�F�3���_��f���lņ��F`sY��t�#<C�������dЎ�s�mݻ6�u�|��oh���8����n��I�����)�!�^g��?�#ʽː*w��S��V�W[��a����_��b��D�%ÌXs��R�.�D��.�Z�3�[�to�k�N��90�EI�9�b/��@�RĠ����\��U�سl���Ɵ�[eԦh�����2e}�!���c���Q0��uWB/�=^�+	�q!��T�Z&Ub�¬�Iu�F§0��H��:���ww����F��;����1o+����8��;�,��l#^��M�k� 0���7=�ώ,ݳ��}���"�j��x�c�����fv�{P���	ڐ�a�d�-���8q���	�����qޥ�h=	��ĩ�!�[DʹH��'Hzb3uI�l�Qq&	�h�)�ҭ.`ԅ��SE��)�B�UF�t�����uG�K~�����p_.����{�A�z��{}J����q�������{�S���ۖ��ݺ�>��{�w�t��c/�{�{o��}5���ˣ&}7��1�?�^���9���t��c/���^���//��u�!����s�{�g�{�~�t���-齾������NH�u@"�s����}�`i ��ʣ�����[���.͸zFC�8��S�e*��p��̩t�����7�\2�|��
��_�[_�~Bo�W�9��!o@��Կ�t��������?,���[� 
�\���lq�v>~�d��ܪ�	�x��+�����������KD��l���~w��_���^�
+�z��~����*ֱ���
������/�������R�7�����?0������ "�F�ֽo8@�o��!E�Ē��kQ|�u�/���M��.<D6f�t_�H\�KZ���&6��}Ays�ž���zh[���q�*~8��&�Y��ԉ��>��%.������7�/齺*��
�����ƿ����̈��1)�C�==x8���o��W�{�K+h�Af��ĺ���vI���C�تջ�Q�1��Y��~"�rB��V�� 6������2�g��D�������R��[��̉������;+���	8ȴA�Ǌ����]-n8ЏᏫ�4�۴������#Cy�2]��5�=>^�#��)9J�kQ7�}� �ź�������3�m!��{����6��t�t�!z���������^޻�e�+�p��v�V��4.��u�3��{N�+0�˳ibp��clp��8��<͋ڟ0��Z���vuo��w�]�л����������6��([��NG�5z��q`��v��_�(Z1s�8�(����'�W�s����e4�T���؀PoFt����n!y��]�h.PX譞������6Y�{6��dm��$ M��,x}���D��PNӎ&cc4H����z�*�d�+��ݨ�b�����tV̋,?=J����l\�U��������� �l>�c�;?��y*X�9�(RO�z%"�4d��>�8���ae��=��Q��Xho������?�iX~���r�z;��߹bF���h�$�ٛa;%�!�,�+q�%��| +�R����tA�G7���/��g����;f�O��珲3FK���hoB������L|�����_��6"L0�ó��3>d�h�����ę�i+ǮU����� -g�e�$�5�l:����/�4�bGL�����}1�{��<y����u�_`}��p;D�_�l�$���i�xI�Ȇ�� ^W���[oa(_�=dܽҙ��)b�����S]t����=�+��qhQ���KuO���w����i���i�ѻhj\d�g�f*�O!�fi� ��X��N-�;�	���ߞ��3�'������D���LW��.@�?����%���L����̶��i���������v��s���%� ���?G�0j��Ky�zЁ����P��Ml��/�O��=Z�8iq�����ꤾ����fs��ڡ4?(���7�y����{���pQp���1�����#x�p��"}�`3"O�Y�	�S�p 󱗊������	�T��n洵���{����O�L/��/���0�>�yI=��b�F���h����kq�;�RH����Yzsa��xlP�hD�w�+b�۴�W�Af��szLJߒXW�b�LG��X����2}-��s̼���
���!u��e��- pΈ��.s=}]._���c���d���ջ�`V�N����s���������	�bD#�Y�Y�����o`R����R�n��?�����Xړ>J� ��%�p�r���#��m����n>�w4=�dW�t~�K�����Nē�����,�OÌ��K��{r����?�Ժ�l.T3^�Ⱥ�u�v`��B|9%������T���ǃ�R���~��2/ۏ�|Ut ���
�?���4�m��:@����&��o���&�{��*�&A��h��S��O�Wn_&��͒3���Bz8O{4?�����|����pﮎQ�]K[���Nt�&�Ǘ����09�qv�yX��n�^i�w�u}�;��G��6��>���}��G\�q��kT9�����������~6���5�pI*�#���գ�4�4jo/;���<��)�8���<�ou����Qz����3��W�w}�9Y����o-e
��y>�1�b����xN�v]N�&�5Y��V�OD�Q������ٚ���Vlxu-?��b2��9���a,�ī�D�������S�؄|�	��$�Ջ���+-������,;�Hz�9�(�6W�t�};>qkD��&w����<FЏ��>jx+��LI승�����;��
����d�O��4��f�AQζ�3�!�_E�t�Y���ˍ��F͎wn®J������g�IWJ>��*0?�۔�f�Y���֯hO�h8~�����a�����)�k�j�_Ur��+�������_���}��Ϭ��N�g]-�sPfp�y`�C�X:��B���99�#����Y�QWÔ��4���E����/XºM��(�pͼ�kk�w8N_��?����M��<�a�|#�O�^�^h5?ʫ��
p�:���n:�9t��eu-��m�n&Xvyfym�/n�?NT�Fl���V��П��f$��<�nc%��	�˟+_	�y:�-�^�h��a(40m��ۛ��ڱW��_�?�-�	������
�cyz>!2^e�/�O|^�[rc�j���<3���|D���\�y'w=� ��r��Y���nF�M�끻�י�6�'c�p�o��/����;{����'+�z��E	���n��xk��R$�HG�s�n����z��W�1��=�9��\O�^��L�ձ���&Qw]���jD���l�8l�g	~�'��e	\��_�̅�gkf�ǣg��s��C��Q�4̊�X92���%�qhޓa�sZ���<�G�
9�Sf+w�fK�^ݮ-�I.@���c��h�s�@����;�˹�9�z]�����LͧDL���ޡZ��3�G���v:���7���h�)�$��M��N��F�½��c�`_�_E� ����o��&Η�;x����]r�/��8��pؐ��w�þ��}����`�_U�o��o�.޿�~�b�2��B���Yx~z֯��ӏ�<��z��u�<�=���y��0"C���/J�!~���z3�w��c>�����c&�-H��v�K.�.W��Uܭ��R�G9�$��@����&<�J�=u?�`�=c~jWz�wƾ�2\U��?������"Z��ᡅ}u��Ώ��>�7Їi�h��pn�?�{$?���>�e����qB���
��c��y6k~s��Ё�	#�p��1�ͼ.�ET�[�7��1\��߽Wb����� �R�
^'�ю�}��]
�9���,�醦��n��I�<�S��!\������s.9��Yw��#�1=�q&��'�G�;A~�26-F�7"�����}�с�0~�n�E��(��~;�r���Y��y�@���;� ���� �ڝ�|�s�<��kP>мmܱ���+
��~�z�mx�ɗ0p����)�/�2�k M�KO�:�����I��[�x�$�8a�[�=�f�[>a��u�b
�L�|�h��4�'�'�}�&�,��uDN�x�-�4��-�v�8#��s-��Yl���"������u)��s�_R���5�Z��}����'H�*�Ӡ��곯w�@�=ep�y��g���=�=/ �:d��ȯ8�͚����� ��+w�c��6_�ŷ�ߔH�n�`&w{�/n���9oE�a��ƞ������|���/�����\�	i�����,�x;4�V���l��O����l}��8$}���>����zvr텃]���;ZAN�w�&��Q �?�m��Jp��mC��A���Eu�>�s�bZ4�v�ҳ��H�@}㵸+:�{G�↫����1�����[��yW�-�� �8ӝ!��'[l��+v��֙��C�H��R�s{P>>;�jS�/L�7���ݏTSo:U�-֗��r<�p~l.	���7��\#.�S�v���sֺ0�N�?�f�൉#F���C}ӏ��O����<�M�����]ө_��X��B���|S3��� �C��q�c�w�s�a"sb��"�;�ߜ鼇s^�!�? �q��k�<��%���$�/�%�fb_�_#�cW>S��r[�v��eƷ���?��Yh��4�1�{���l�~�����{�կ�[���?!�h��K1vω���}�����Av��x=���)��W�D�[�{���GU�?        RSRC                    PackedScene            ��������                                            N     ..    OtherWorld    tests_use_threads 	   LinePath 	   Spatial2 
   transform 	   Spatial5 	   Spatial4 	   Spatial7 	   position    SubViewport    RayEmitter    .    Lines 	   OpenFile    MusicPlayer    stream    resource_local_to_scene    resource_name    length 
   loop_mode    step    capture_included    markers    tracks/0/type    tracks/0/imported    tracks/0/enabled    tracks/0/path    tracks/0/interp    tracks/0/loop_wrap    tracks/0/keys    script    _data    sky_top_color    sky_horizon_color 
   sky_curve    sky_energy_multiplier 
   sky_cover    sky_cover_modulate    ground_bottom_color    ground_horizon_color    ground_curve    ground_energy_multiplier    sun_angle_max 
   sun_curve    use_debanding    energy_multiplier    sky_material    process_mode    radiance_size    background_mode    background_color    background_energy_multiplier    background_intensity    background_canvas_max_layer    background_camera_feed_id    sky    sky_custom_fov    sky_rotation    ambient_light_source    ambient_light_color    ambient_light_sky_contribution    ambient_light_energy    reflected_light_source    tonemap_mode    tonemap_exposure    tonemap_white    ssr_enabled    ssr_max_steps    ssr_fade_in    ssr_fade_out    ssr_depth_tolerance    ssao_enabled    ssao_radius    ssao_intensity    ssao_power    ssao_detail    ssao_horizon    ssao_sharpness    ssao_light_affect    ssao_ao_channel_affect    ssil_enabled    ssil_radius    ssil_intensity    ssil_sharpness    ssil_normal_rejection    sdfgi_enabled    sdfgi_use_occlusion    sdfgi_read_sky_light    sdfgi_bounce_feedback    sdfgi_cascades    sdfgi_min_cell_size    sdfgi_cascade0_distance    sdfgi_max_distance    sdfgi_y_scale    sdfgi_energy    sdfgi_normal_bias    sdfgi_probe_bias    glow_enabled    glow_levels/1    glow_levels/2    glow_levels/3    glow_levels/4    glow_levels/5    glow_levels/6    glow_levels/7    glow_normalized    glow_intensity    glow_strength 	   glow_mix    glow_bloom    glow_blend_mode    glow_hdr_threshold    glow_hdr_scale    glow_hdr_luminance_cap    glow_map_strength 	   glow_map    fog_enabled 	   fog_mode    fog_light_color    fog_light_energy    fog_sun_scatter    fog_density    fog_aerial_perspective    fog_sky_affect    fog_height    fog_height_density    fog_depth_curve    fog_depth_begin    fog_depth_end    volumetric_fog_enabled    volumetric_fog_density    volumetric_fog_albedo    volumetric_fog_emission    volumetric_fog_emission_energy    volumetric_fog_gi_inject    volumetric_fog_anisotropy    volumetric_fog_length    volumetric_fog_detail_spread    volumetric_fog_ambient_inject    volumetric_fog_sky_affect -   volumetric_fog_temporal_reprojection_enabled ,   volumetric_fog_temporal_reprojection_amount    adjustment_enabled    adjustment_brightness    adjustment_contrast    adjustment_saturation    adjustment_color_correction    tracks/1/type    tracks/1/imported    tracks/1/enabled    tracks/1/path    tracks/1/interp    tracks/1/loop_wrap    tracks/1/keys    tracks/2/type    tracks/2/imported    tracks/2/enabled    tracks/2/path    tracks/2/interp    tracks/2/loop_wrap    tracks/2/keys    tracks/3/type    tracks/3/imported    tracks/3/enabled    tracks/3/path    tracks/3/interp    tracks/3/loop_wrap    tracks/3/keys    code    viewport_path    render_priority 
   next_pass    shader     shader_parameter/albedo_texture    lightmap_size_hint 	   material    custom_aabb    flip_faces    add_uv2    uv2_padding    size    subdivide_width    subdivide_depth    center_offset    orientation    radius    height    radial_segments    rings    subdivide_height    custom_solver_bias    margin    transparency    blend_mode 
   cull_mode    depth_draw_mode    no_depth_test    shading_mode    diffuse_mode    specular_mode    disable_ambient_light    disable_fog    vertex_color_use_as_albedo    vertex_color_is_srgb    albedo_color    albedo_texture    albedo_texture_force_srgb    albedo_texture_msdf 	   metallic    metallic_specular    metallic_texture    metallic_texture_channel 
   roughness    roughness_texture    roughness_texture_channel    emission_enabled 	   emission    emission_energy_multiplier    emission_operator    emission_on_uv2    emission_texture    normal_enabled    normal_scale    normal_texture    rim_enabled    rim 	   rim_tint    rim_texture    clearcoat_enabled 
   clearcoat    clearcoat_roughness    clearcoat_texture    anisotropy_enabled    anisotropy    anisotropy_flowmap    ao_enabled    ao_light_affect    ao_texture 
   ao_on_uv2    ao_texture_channel    heightmap_enabled    heightmap_scale    heightmap_deep_parallax    heightmap_flip_tangent    heightmap_flip_binormal    heightmap_texture    heightmap_flip_texture    subsurf_scatter_enabled    subsurf_scatter_strength    subsurf_scatter_skin_mode    subsurf_scatter_texture &   subsurf_scatter_transmittance_enabled $   subsurf_scatter_transmittance_color &   subsurf_scatter_transmittance_texture $   subsurf_scatter_transmittance_depth $   subsurf_scatter_transmittance_boost    backlight_enabled 
   backlight    backlight_texture    refraction_enabled    refraction_scale    refraction_texture    refraction_texture_channel    detail_enabled    detail_mask    detail_blend_mode    detail_uv_layer    detail_albedo    detail_normal 
   uv1_scale    uv1_offset    uv1_triplanar    uv1_triplanar_sharpness    uv1_world_triplanar 
   uv2_scale    uv2_offset    uv2_triplanar    uv2_triplanar_sharpness    uv2_world_triplanar    texture_filter    texture_repeat    disable_receive_shadows    shadow_to_opacity    billboard_mode    billboard_keep_scale    grow    grow_amount    fixed_size    use_point_size    point_size    use_particle_trails    proximity_fade_enabled    proximity_fade_distance    msdf_pixel_range    msdf_outline_size    distance_fade_mode    distance_fade_min_distance    distance_fade_max_distance    interpolation_mode    interpolation_color_space    offsets    colors    default_base_scale    default_font    default_font_size    content_margin_left    content_margin_top    content_margin_right    content_margin_bottom 	   bg_color    draw_center    skew    border_width_left    border_width_top    border_width_right    border_width_bottom    border_color    border_blend    corner_radius_top_left    corner_radius_top_right    corner_radius_bottom_right    corner_radius_bottom_left    corner_detail    expand_margin_left    expand_margin_top    expand_margin_right    expand_margin_bottom    shadow_color    shadow_size    shadow_offset    anti_aliasing    anti_aliasing_size 	   _bundled       Script *   res://examples_dd3d/DebugDrawDemoScene.gd f�s�!	   FontFile $   res://examples_dd3d/Roboto-Bold.ttf j	-��b	   FontFile *   res://examples_dd3d/PixelatedElegance.ttf i
��    Script ,   res://examples_dd3d/demo_camera_movement.gd �qܡܯ>   Script -   res://examples_dd3d/demo_music_visualizer.gd E�3;J%   Script +   res://examples_dd3d/demo_settings_panel.gd �E{����    Script 4   res://examples_dd3d/demo_web_docs_version_select.gd �z����p"      local://Animation_ucqh5 �!         local://AnimationLibrary_cq37i �"      $   local://ProceduralSkyMaterial_87638 �"         local://Sky_4jfme L#         local://Environment_38m85 p#      
   local://9 �#         local://10 �(         local://AnimationLibrary_nj4nv �,         local://Shader_621vv -         local://ViewportTexture_vpw84 �-         local://ShaderMaterial_ho0aq .         local://PlaneMesh_c6mie h.         local://CapsuleMesh_tigpa �.         local://BoxMesh_b14rm �.      
   local://4 �.      
   local://5 /      
   local://6 �/      
   local://7 *1         local://AnimationLibrary_vh8ml D2      !   local://StandardMaterial3D_rbfyn �2         local://QuadMesh_1t0id 3      
   local://1 <3      
   local://8 �3         local://AnimationLibrary_a7f1a �4         local://Shader_3cmiq �4         local://ShaderMaterial_t3isk �5         local://BoxMesh_0xv07 36         local://Gradient_tup4c [6         local://Animation_n750a �6         local://AnimationLibrary_0ity1 �7      
   local://3 8         local://StyleBoxEmpty_oj5gf 18         local://StyleBoxFlat_boyhr g8         local://PackedScene_vo7pc �8      
   Animation 
            RESET       o�:         value                                                                    times !                transitions !        �?      values                    update                AnimationLibrary              ,      RESET                    ProceduralSkyMaterial    "      �p%?;�'?F�+?  �?(      �p%?;�'?F�+?  �?         Sky    /                     Environment    8            @         x      ���>z      ��=}         ?~      �@      
   Animation           	   New Anim         �?                  value                                                                   times !          333?      transitions !        �?  �?      values            �?              �?              �?   @  �?  �?     �?              �?              �?   @���>  �?      update        �         value �          �         �              �         �         �               times !             ?      transitions !        �?  �?      values            �?              �?              �?   �  ��  �?     �?              �?              �?   ��@ʿ  �?      update        �         value �          �         �              �         �         �               times !            �?      transitions !        �?  �?      values            �?              �?              �?-%�    _��?     �?              �?              �?-%��J�_��?      update        �         value �          �         �           	   �         �         �               times !      ���>  �?      transitions !        �?  �?      values          q=�?F����̼   �9�?u�	lμ      update              
   Animation          o�:         value                                                                   times !                transitions !        �?      values            �?              �?              �?   @  �?  �?      update        �         value �          �         �              �         �         �               times !                transitions !        �?      values            �?              �?              �?   �  ��  �?      update        �         value �          �         �              �         �         �               times !                transitions !        �?      values            �?              �?              �?-%�    _��?      update        �         value �          �         �           	   �         �         �               times !                transitions !        �?      values          �9�?u�	lμ      update                 AnimationLibrary              ,   	   New Anim          ,      RESET                   Shader    �      �   shader_type spatial;
render_mode unshaded;

uniform sampler2D albedo_texture : source_color;

void fragment() {
	ALBEDO = texture(albedo_texture,UV).rgb;
}
          ViewportTexture    �            
            ShaderMaterial    �          �          �            �         	         
   PlaneMesh    �         
   �   
     �@  �@         CapsuleMesh    �      q=�>�      ���?         BoxMesh             SphereShape3D    �        �?         StandardMaterial3D    �         �      ��?��?��:?���>�         �      ��@?��=?��\?  �?      
   Animation          	   New Anim         @@                  rotation_3d                                                     !            �?              �?ff�?  �?�$�55@�����  �?33@  �?T8"�za=�{?g*佔         position_3d �          �         �            �         �         �   !            �?!����@�Q�   ?  �?X7j?,}�?!Խff�?  �?�r<�@v�33@  �?��?�ɳ?O�?      
   Animation          o�:         position_3d                                                     !            �?!����@�Q��         rotation_3d �          �         �            �         �         �   !            �?              �?         AnimationLibrary              ,   	   New Anim          ,      RESET                   StandardMaterial3D    �         �         �          �      ��\>��\>��\>��C?      	   QuadMesh    �            �                  StandardMaterial3D    �          �      ��?���=���=  �?      
   Animation 
            RESET       o�:         value                               	                                    times !                transitions !        �?      values            �@   �          update                 AnimationLibrary              ,      RESET                   Shader    �      �   shader_type spatial;
render_mode unshaded;

uniform sampler2D screen_texture : hint_screen_texture, repeat_disable, filter_nearest;

void fragment() {
	vec4 col = texture(screen_texture, SCREEN_UV);
	ALBEDO = col.brg;
	ALPHA = col.a;
}
          ShaderMaterial    �          �          �                     BoxMesh    �                  	   Gradient    -  !      吚;�ϡ>o%?  �?.  $          ���<  �?  �?�h�=  �?^��=  �?  �?�X�=���=  �?    ���<  �?  �?      
   Animation 	         o�:         value                                                                       times !                transitions !        �?      values                update                AnimationLibrary              ,      RESET                   Theme             StyleBoxEmpty    3       �@5       �@         StyleBoxFlat 
   2       �@3       �@4       �@5       �@6     ���=���=���=��H??        @        A        B                 PackedScene    M     	         names "   �      DebugDrawDemoScene    process_priority    script    custom_font    custom_3d_font    text_groups_position    Node3D    RESET 
   root_node 
   libraries    AnimationPlayer    DirectionalLight3D 
   transform    visible     directional_shadow_max_distance    WorldEnvironment    environment    Camera 
   cull_mask    current    fov    far 	   Camera3D    Panel    custom_minimum_size    anchors_preset    anchor_top    anchor_bottom    offset_top    offset_right    grow_vertical    PanelContainer    ViewportContainer    layout_mode    SubViewportContainer 	   Viewport    handle_input_locally    size    render_target_update_mode    SubViewport    CameraLayer2_5    near    Zones    SpheresBox    unique_name_in_owner    CylindersBox 	   BoxesBox 	   LinesBox 	   PathsBox    MiscBox 
   LinesAnim 	   autoplay 	   LinePath    Spatial 	   Spatial2 	   Spatial3 	   Spatial4 	   Spatial5 	   Spatial6 	   Spatial7 
   Cylinders 
   Cylinder1 
   Cylinder2 
   Cylinder3    1    2    Spheres    SphereTransform    SphereHDTransform    SpherePosition    Boxes    Box1    rotation_edit_mode    Box2    Box3    AABB_fixed    AABB    a    b    BoxAB    o    metadata/_edit_group_    up 
   BoxABEdge    OtherWorld    mesh 	   skeleton    MeshInstance3D    own_world_3d    anchor_right    grow_horizontal    stretch    OtherWorldBox    Misc 
   Billboard    Arrow 	   Position    GizmoNormal    ZDepthTestCube    GizmoTransform    GizmoOneColor    HitTest    StaticBody    StaticBody3D    CollisionShape    shape    CollisionShape3D    HitTestSphere    radius    radial_segments    rings 	   material    CSGSphere3D    RayEmitter    RayCast    enabled    target_position 
   RayCast3D 	   RayCast2 	   RayCast3 	   RayCast4 	   RayCast5 	   RayCast6    RayEmitterAnimationPlayer    Grids    GridCentered    Subdivision    Grid    PlaneOrigin    Lines    3    4    5    6    7    8    Target    LagTest 	   CSGBox3D    PostProcess    MusicVisualizer    offset_left    offset_bottom    colors    VBoxContainer 	   OpenFile    size_flags_horizontal    text    Button    MusicPlayer    bus    AudioStreamPlayer    VBox    HBoxContainer    VolumeSlider    size_flags_vertical 
   max_value    step    value    HSlider    MuteMaster    button_pressed 	   CheckBox    AudioVisualizer    CustomCanvas    anchor_left    mouse_filter    metadata/_edit_lock_    Control 	   Settings    theme    switch_to_scene    HBox    VersionBlock    Label $   theme_override_font_sizes/font_size    OptionButton    item_count    popup/item_0/text    popup/item_0/id    theme_override_styles/normal    horizontal_alignment    metadata/_edit_use_anchors_ 
   alignment    HideShowPanelButton    SettingsPanel    theme_override_styles/panel !   theme_override_colors/font_color    HBox3    ThicknessSlider    HBox5    FrustumScaleSlider    UpdateInPhysics    Label4 	   ShowText    ShowExamples 
   ShowStats 
   ShowHints    Draw3DText    Label3    HBox4 
   DrawBoxes    Draw1MBoxes    tooltip_text    DrawBoxesAddText    SwitchLang 	   _pressed    pressed     _on_volume_slider_value_changed    value_changed    _on_mute_master_toggled    toggled    _on_hide_show_panel_pressed #   _on_thickness_slider_value_changed '   _on_frustum_scale_slider_value_changed    _on_update_in_physics_toggled    _on_show_text_toggled    _on_show_examples_toggled    _on_show_stats_toggled    _on_show_hints_toggled    _on_draw_3d_text_toggled    _on_draw_boxes_toggled    _on_draw_1m_boxes_toggled    _on_add_text_to_boxes_toggled    _on_Button_pressed    	   variants    �                                                              ,                   ��]�F�ݾ" �>    ���>�]?2  ���??}�ݾ                        HC              �?            St?>К>    >К�St?������/A���A           TB     �B         
     �C  �C     �?     ��     �C       -   ,  ,       �?            ?�>��p?    ��p�?�>d;�`�B�`A   33B   �Q(@     RB   �A            
��@            U�A�2�!X%?����   ^�A            $�@            P��@+v��t����'��    � A            ���@            ��AA�4��W�S����@   c+A            ��~@            ��@M*A��?3���   �r�@            ��@            �@c`=>�|�?r���   �q�@            ".@            hy�@P��W�S����@                   ,                   	   New Anim      �?              �?              �?    �yC@   �     �?              �?              �?   @  �?  �?     �?              �?              �?H��>      @@     �?              �?              �?-%�    _��?     �?              �?              �?   �  ��  �?     �?              �?              �?      ��  ��     �?              �?              �?�9�?u�	lμ     �?              �?              �?z6�����4�N��   ���?Lk?MU����?A�����[?U>�)پnQ��sB�    �y��     �?              �?              �?�p>5�ؾ�~�>     �?              �?              �?��@S�'�w���     �?              �?              �?}�־ ������     �?              �?              �?�M�?�:�>�ũ?     �?              �?              �?��1���*>u���   �&A@            $	r?QS�    zǅ?�(?@�A	����47�@   ��?�H�?����H�=�2O?��@`�)�M�?�#���n����4(�     �?              �?              �?�;�?���>e��?     �?              �?              �?"�s�    *t�@   �9@�'
�����;cb{@�g<�j?����E�?�(v�    �Yr�     �?              �?              �?
,�r�����?     �?              �?              �?�K����	��@   E�U?    ��      �?    �?    U�U?�m���O��>     �?              �?              �?���?0�^�o�Q>   �5]?�t�>��> ����[?ٔ��� �0f�>��=? �?�����?   �5]?�t�>��> ����[?ٔ��� �0f�>��=?C➿B&�?�b�     �?              �?              �?!Y*@)��K�@   �5]?�t�>��> ����[?ٔ��� �0f�>��=?�^?{�*� �s?   �5]?�t�>��> ����[?ٔ��� �0f�>��=?k~��7?�2q�   ~�S?p��>�>��ҽ2Z?ٔ�������>��=?                 �? ��    ��߳  �?              �?    m�?2穾   �Fw?�0��  ��    �Fw?탄�탄>  �>��n?!<�>�����)�@   1�v>��&?�Z8?�����A?Hk$�~pv����=Qg�>���>X�"���=   1�v>��&?�Z8?�����A?8k$�~pv�=Qg�>1C���v?A�     �? �2���2���1  �? �� �� ��  �?                 �?����    ����  �?    ���2 ��  �?�����?         �?              �?              �?��@   �\��@                                 �?              �?              �?�B�@��?�-�@     �@     �?              �?              �?�B�@��?��@                                �?              �?              �?�׵�    ��@     �?              �?              �?I��>Xǩ�Hm@   YM?!����&��
���<�jW����>-u?]��Â�=�+�d�@   ��?��?&�?ף���)?%�?    ���":@�Z?%��='N޿   �Fw?    탄�      �?    탄>    �Fw?X�c?G̜�yZ�>   E�?     @3    E�?     @�    E�?                        �?a?�h~>�eϾv��#/k?��A�~p�>�6�>��d?��V��~��4�   +i�>I<*=Y�k?w��=�N~?߻��O@k�3�=dW�>��V�m��C��     �?              �?              �?�o9?l!��LT�@                  
                 �?              �?              �?!����@�Q�     �?              �?              �?+k;�    -�P�       -�]�         �?              �?              �?�S/?    {�J�     �?              �?              �?x_5�    �?     �?              �?              �?�_5?    7?   Ify?[g�    [g>Ify?              �?'�    ���   ,�o?<�>    <��,�o?              �?�\�>B�{��X��      ,                   �5?    5�      �?    5?    �5?h;?   ���0@   �Y�?�I9?�Y߿#���襳�����Y߿�I9?�Y�?u��?������?     �?���� �����*  �? �(����! *  �?��L����4���>     �@    ���4      �?    ����      �@                 �?    ����    ��? �7    ���/��?  �?      �?     �?            1�;�  ��      �?1�;�m�0A*��@w�R�            ��?��?&�?ף���)?%�?    ���":@�#AXǩ�TR%�     �??R�1���2֔|5  �?    �'�� ��  �?'�����x�?     �??R�1���2֔|5  �?    �'�� ��  �?ff����E�&�     �??R�1���2Ր85  �?    q��� ��  �?@��4hо6aV=     �??R�1���2�,5  �?    �Q� ��  �?�˗?��?l��>   ��L��߫�I.�=EG�,�&�^<Y��u�?�3�?�[�>D4�?   ��L��߫�.�=EG�,���^<Y����?���?���?Ƣ�?   $��L�����W-�=EG�,�h�^<Y����?MXQ=�k��4��?   $��L�����"-�=EG�,�?�^<Y����?����Ԃs�:�?     �?a�7�\��44��6  �?    � ��  �?��0�ݵ4>*�?     �?              �?              �?  �@   �          @   @   @               ,                   ~�j�      �?      �@      ��    �͌�  �A                                    A     HB                        Open music                    ,                ,      MusicAnalyzer 
     �B          )   {�G�z�?)   �������?      Mute    ��L>              �@            ��L>x����q=bA    @�     8B     �    �D                     .   res://examples_dd3d/DebugDrawDemoSceneCS.tscn     ���     ��   u �      �                           Demo version:       1.0.0                GDScript example       Hide panel              ��J?��J?��J?  �?      Common:       Thickness        ?)   ����MbP?)   �������?      Frustum Scale       Update in physics (15 Ticks) *       Text:    
   Show text    	   Examples       Debug stats       Hints       3D Text       Boxes:       Draw an array of boxes    )   Draw 1 Million boxes, otherwise 7500pcs.       1M       Add text to boxes       Switch to C#       node_count    �         nodes     �  ��������       ����                                              
      ����         	                        ����                  	                     ����      
                     ����                                                          ����                                                              "       ����   !                 '   #   ����   $      %      &                    (   ����                           )                           *   ����        	          +   ����   ,                	          -   ����   ,                	          .   ����   ,                	          /   ����   ,                	          0   ����   ,                	          1   ����   ,                        
   2   ����          	   !   3   "                  4   ����      #                 5   ����                  6   ����      $                 7   ����      %                 8   ����      &                 9   ����      '                 :   ����      (                 ;   ����      )                  <   ����      *                 =   ����      +                 >   ����      ,                 ?   ����      -                 @   ����      .                 A   ����      /                  B   ����      0                 C   ����      1                 D   ����      2                 E   ����      3                  F   ����      4       #          G   ����      5   H          #          I   ����      6       #          J   ����      7       #          K   ����      8       #          L   ����      9       (          M   ����      :       (          N   ����      ;       #          O   ����      <       +          M   ����      =       +          N   ����      >       +          P   ����      ?   Q          .          R   ����      @       #          S   ����      A       0          M   ����      B       0          N   ����      C       0          P   ����      D   Q          3          R   ����      E               W   T   ����      F   U   G   V   H       5       
      ����   	          5       '   '   ����   X      $      &   I       7       "   "   ����      J   Y            Z            [          8       '   '   ����   $      &   I       9             ����      K            L       9       W   W   ����      M   U   N   V   O       9          \   ����   ,         M                  ]   ����      P       =          ^   ����      Q       =          _   ����      R       =          `   ����      S       =          a   ����      T       A       W   b   ����   ,         U   U   V       =          c   ����      W       =          d   ����      X                  e   ����      Y       E       g   f   ����        F       j   h   ����   i   Z       F       p   k   ����   ,      l      m   [   n   \   o   ]       E          q   ����      ^       I       u   r   ����      _   s      t   `       I       u   v   ����      a   s      t   `       I       u   w   ����      b   s      t   `       I       u   x   ����      c   s      t   `       I       u   y   ����      d   s      t   `       I       u   z   ����      e   s      t   `       E       
   {   ����   ,      	   f   3   "                  |   ����      g       Q          }   ����      h   H          R          ~   ����      i       Q             ����      j       T          ~   ����      k               W   �   ����      l   U   m                  �   ����      n       W          @   ����      o       W          A   ����      p       W          �   ����      q       W          �   ����      r       W          �   ����      s       W          �   ����      t       W          �   ����      u       W          �   ����      v       W          �   ����      w               �   �   ����      x   %   y   o   z       a       
      ����   	   {               W   �   ����      |   U   }   V   ~               �   �   ����   �               �   �   �      �   �   �       d       �   �   ����   !      �      �   �       d       
      ����      �   	   �       d       �   �   ����   ,      3      �   �       d       �   �   ����   !          h       �   �   ����   !          i       �   �   ����   ,         �   !      �   �   �   I   �      �   �   �   �       i       �   �   ����   ,      !      �      �   �                  �   ����   ,         �               �   �   ����   ,      !   �          �      Y      �   �      �      �   �   �   Z      �      �                  �   �   ����   !   �      J   Y            Z            �      �   �      �   �   �   �          n       �   �   ����   !          �   �            Y            �   �      �      �   �   �   Z                o       �   �   ����   !      �   �   �   �       p       �   �   ����   ,      !         �       q       �   �   ����   !      �   \   �   �   �   �       q       �   �   ����   !      �   �   �   �   �       �   �   �          p       �   �   ����   !      �   �   �   �   �   �   �   �   �      �          o       �   �   ����   !      �          u       �   �   ����   ,      !      �   I   �   �   �   �       u          �   ����   ,      !      �   �   �   �   �   �       w       �   �   ����   !      �   �   �          x       �   �   ����   !      �   �   �   �       x       �   �   ����   !          z       �   �   ����   !      �   �       z       �   �   ����   ,      !      �   �   �   I   �   �   �   �   �   �       x       �   �   ����   !          }       �   �   ����   !      �   �       }       �   �   ����   ,      !      �   �   �   I   �      �   �   �   �       x       �   �   ����   ,      !      �   �       x       �   �   ����   !      �   �   �   �       x       �   �   ����   ,      !      �   �       x       �   �   ����   ,      !      �   �       x       �   �   ����   ,      !      �   �       x       �   �   ����   ,      !      �   �       x       �   �   ����   ,      !      �   �       x       �   �   ����   !      �   �   �   �       x       �   �   ����   !          �       �   �   ����   ,      !      �   �       �       �   �   ����   ,      !      �   �   �   �       x       �   �   ����   ,      !      �   �       x       �   �   ����   ,      !      �   �             conn_count             conns     p   e   d   �   �              j   d   �   �              k   d   �   �              v   n   �   �              |   n   �   �                 n   �   �              �   n   �   �              �   n   �   �              �   n   �   �              �   n   �   �              �   n   �   �              �   n   �   �              �   n   �   �              �   n   �   �              �   n   �   �              �   n   �   �                    node_paths              editable_instances              version             RSRC        [gd_scene load_steps=3 format=3 uid="uid://sxtw8fme7g63"]

[ext_resource type="PackedScene" uid="uid://c3sccy6x0ht5j" path="res://examples_dd3d/DebugDrawDemoScene.tscn" id="2"]
[ext_resource type="Script" path="res://examples_dd3d/DebugDrawDemoSceneCS.cs" id="2_ipqea"]

[node name="DebugDrawDemoSceneCS" instance=ExtResource("2")]
script = ExtResource("2_ipqea")

[node name="Settings" parent="." index="23"]
switch_to_scene = "res://examples_dd3d/DebugDrawDemoScene.tscn"

[node name="Label" parent="Settings/HBox/VBoxContainer" index="1"]
text = "C# example"

[node name="SwitchLang" parent="Settings/HBox/VBox/SettingsPanel/VBox" index="12"]
text = "Switch to GDScript"
              GDSCd   $  (�/�`$�5 ���J�XQ̵�8$&ՕA(!\B�H�s\��V[�{�{���!�i�3�<�����˚'�D��eD�v{8���������� e � +�S=-�0��$��eR�G��x=<�qC�w��kI�)C��.�ݥ������+N��d�fN3J�J���f��#���O����[n�����e �񴴻3�ow�v�.��Q~��6��4v��]�봻P��o��o;��_���9?�6���ŏ�M��m~���=l��~��o�-�����S�i��_���G�� ���'��o���e��#��w�]��
%)B�(����`�@ e�`6Z+�x b>�֚U�R|���$-c�'�&��|�W-EW}�zQ˥ҍ�V�EJGY(��ҩKU�.�Ug��J)�tR���\.:����/梓�颓�9眳�煟�'������������I~�?H�� �D$�g6�D = �BD��h($"w�R(�*�;� H
k �?�H�=u���Sw���KP�v~�D�0��8��H�	qH�
,��oj.�< WS�tI��͎�<P_�r7���)m�5;�r��8�ۭ&�F�j�51�I*PC@\���6�B�@A["
�!����OFIR�Ja��#QJi�&HZ��DDDAJST)m�h� c���\]`��1[��*��64�Mn�j�����K({��]�+ܖ�:��~��U��ȍ.k�NI��.��Cϴ��'�z{J�J������1�طq)��){Џ�������?�[��M6a<��2��Z��Hs��^�بn0��R�^�uV��Y��fG'�]��B߮�u@;��{90s���_����ΊZg ��F"د�?3@��@ �e��dF����۔�ol c/2�^��|��~8#��o�+uf���(Ы���ߩY����M� ~�U}gX��X`]�ɯ�"3ޱ�*�Ӧ�Pdѭ�H��ī��^��>P|E�l���]�
��B�@N�A�С�L�~rc=Ĥ/9�f����}vWiR��˜<����ȯe��d����\A�v�,W�b�=����)��o��n|Xd�ۢ,��~�ĩ��8 gf��nj�;���U\V��W��m �T��$�M��l���L��{9R=���#
�N�bj�w[��c��L��܎�e��OQ��?�Y�]��^!0������|��=�9y�w	x��ʁ�g�5�'S~��<̫����sr;���o��$�n_71���}���8�}����x��)n&~����7ϰ�9w�:P�	�>~H�!��5���A~�&��g	��ܖ�A�r.૾o��	�&�,��0\m�{<z�������Ko���^���PP�Ųi�l��|�y�����O��h��Wa���v)������=�ީ������W�����B]���e���$L~�&jm���3$��7~W������70\g�����f*FE�`y��u�A���G�imO��h��_���򣼏�~Y_?}�<���<��o8x`v�%�_��%������@��ʻuS|»'�=:�|��~+^�-�����2>�Zm��MvԇE��y��y��	�ΐ�7�����j<أM���	���i���P���4�i�_ڶ>���X��+q�O�wO�-�O��${twxK~ޜ����W*+����\���     GDSCd   LI  (�/�`LHū ���)L0�2Ic���`=�
l[�#Ѷ��|��ֹ�����9�[���`��%b�=��?�7���GZ�X^�j�����!)��TQSTS:,tZ��"=�@���`J�e����o*��,ZX�����6���8�\U]Q}����N��J�E�)-���D票�<�H	TY[9,--�-]����\�.�EF�H�����-'N9-�*FE�����䴰�rY[R9���-]�	W��n+�J�_��*T�Г/Nj���J+KG��K����	���-)��_����I�3��ÚQ_r�|�{��7�kma]U��sg����OY�#mM��ǜ�3DaΙ��Z=�^<��2�446=�ק$r�Q������gP�@\�lMn$����~�3���?Q:d�5%�uH+R�s��!���>_.��]��a+��F;77:}pl0���$�jb�I��[��[�K�s��?T�9��5"���:?�gO��p��%��m�{�	"D�Ӝ�N�6�@sNB��(�6�tb��Gi�����툭,�nПg6����'����i�c��I�c��ў���-~�<J2K;ZC*���� u��(#{����9��7:6ʶ4;t����M������[�c�I6�Cio3� :ҳ+#ڣ����/ɘs��)"���
Ď�_d�s�BR�9��b�9��l-��AsN5.�e��h�iF����7��F3$�%�jed�662>��ۍ={j��9��̘�s�����d?8�bΙ��4��g�iF�ǰ�f��7"����9g͢l�Y��9�ذ�s�+�L�3�{��ds�,��Ǚ�	� �@��p� �x\�_���g���qT~����qB~���qI?�exp�?�A?����`?�����������/`��Y~������̓@~������_�o?
�����w������WĿ��������/�������g�������σ��O�T~����W�E������_?����U�5������򟑟�!��A~,�-�#�G��?�����+���s�����3xo	��z/���$~*�����R�#��=9���<��~	�s�Cyo��{i��#�q�N����޳�����;����{���o�^���{D����+yO�O�^я��W���?�������Q����~~=�ӏ����A~$�?�/�q���y����K�?�f�����_��#�S(x��K��1~ ������p� ��.C�$iW�@JJZ���D�3OR4a�����t�)�)�UK+7EeU����8���6Ѳ��
▽Բ��,{��pڴx��3(�I�e�S֡P͕�NB-]���Z���E���R-��S;�m+�cT�e'��Z��K.�MJ�}��F�ɨ�NE�ɨ�;BB��
�`�.z��Q�A�]�`'��J�]�`'�v)\�`'���S+[��d�/������#�b��D��&��vr����T�3~V�N/��%[�)���`gC�.V�H���}e/)X��xv���&` |�N%����B����bjw�`'���>�:�Ivj��;&;u`'�E���d�N�j������%;�W���U�d/��$���
�#;�CL`I#[s���x@id�Z�@Y��*�_��^֤�@Y�z��H� �@t"ذz��HT�CBٰ:�z���r9��i�^�W�cŮ6v��I��c��Jر�P䱣�P�ܱ#��E<Lg �K;��:�2ǎ5��RiG�p�LڑF�X��qF�X��QF�X���(k�H;��2
��X����.�2���B�u�/���J�
�]�p�4��
�4FW�N�ذ9?�P������W/܅���ֈ��PY��D�.	4���ED��*KW� it��۟3:E�ѡ0/4f��<�BeJ(|N�$����|K4>Ћ+yPx}�`KK�N"䈉1�Y����UO������#�C����TE����A�5gƄ�E
�+U�P�dI���a���i;�c�+;�a�Nk�]���ڰ�>��:���}L��é3,��W�a�컢pJ�>�\�� þc�Kq�$�cN!�l�a��R�ʴ�`;������z�
w@Bf�� �mV�וR!�����^��#��0�W����~�d�V C�;¿����)�]t��o���^�V���v�N"�⛽D��`��V�|�c%�B��
T�u�k�1K��AB3K��x��]�W���̊�-�c%
�����_ev�3+^���2��2+^�ّ�.C����.2��
��f��-m�	}3Z�k�&�ݍg�������L{�fwC��f��^J;U��L{��>VzBE���0;M��9v�i/��cő��*H��)	�ȴ��>¹��rw,k���p��q�?��ˮ}$
k=�~U^�W
gR̊�g��̘��⥙},3f'ŬxYf���Z��>r��I1+^r�#n�K1;^�����KI�f�a	�h���S����E0�X֩��8��])�%Ad*��X�p/pw�k?�j��N��$�=��.��I${�ȅD��=��N���˶Y�9�\8m�K���m[��I.[���.r�K�j�v��p��^r�Ǻj�5�˪�>��U-�+;�e��G8�`'���k��2F9""�H�$I��@�`(��D�@��V�,�1ʈ�2R���R��;�p��t�/�q�h�ԄdY9��H�cl�$t��A���m�?�A~?�D@l��B����>��4Tر��('�=
���e��U<�g�*Ąz�6O�6r�v}�><X����h�a^l��j�"I�]65�I̤P"BF ���Ĥ͊�����;8g��b�݂��0�v���g;����}gZ���\ �=| �|��#aO�A0X��Ҿ8������o��[��J�e���V���BB��ރ��ι2�=f�K�;��*)m0�%�H���Zr�2�)��v����-B�@���*`��F^8�hM >����y�I���2�5v��]ai?���K���Y�Gr�ɱ]*RB�F��4���"��:d�mm�e���뛪{����l�R� 覿6�=���a­U�s�v �Ѡ�"l�W�C�k�;3�|��n�����\��	�Dӫ/J�W����m�l��Z
[���:�������2�s���{��Mc�)�U^��&+�������MoY��6S&�igM<k�$�±��[�\T��ǚ0�͑S{��ªzm�Yhʯ���X�V�۽o��](Q��X�̗)��I�z��7J��S���i��k�d^�!��hz��^�������<��_,��j��s�H��T/���q���L������������cPd�����v7eE���56�e�pQ��)��P7�=�o0L�6�lxCau���*�.�O9�?���)��S����V����h�j��K��2��R�Y8f��ݔ�/j��p}u�j�K_[�]�wm���ٳ��D�y���dS���g�y:#��ߤ[�/��y�j������r���
�g���TuԚ��.3M�7s2Pe�{�j_⭖w�I'8��[�d�P`���0����/��/<��6��M!|�-�x�܆���i7��`��H]���J��o�'?�{R��>{Z�o�S�-��q�[I��2�	��Ot�|��c�0���1?a"�YX4I�i�c�ne�����)&��o�c�q,&�'sB{�ff� u:R��7�ؚ�O���������Jb�s�?��rc��� �'��"��]�h�nާKܐ��<�F0�gf6�n�3���3���1�i0(���4o��Ձ96`��Ӌ��L������X���{�(�{�'�m/��m�[��i�~;{>�1W.U�Ft9��R��;��VYs�`�	'>Q�����D祶����C'��u��9B+�?��Ի����5���_����^��/_����'�}��y��s�o��Q�>�T���Z|la�<��r_`�͂#�2a����Hj 딼�������T�[�v�Hҥ��x��W��S���g�� =�sEr��*Z���R����}�}�[�K�u�B���mO |*��^����v��ց6wG����,X����e,�}!�xy��\�c9�Z=/�[����R����A�y�$�knܹ��KQά����jO�Y�D�i����п�!w8f������x����t��H�>��������τ�0zx;�G�B~Y�>_�����q4l1���R/�Y��qsڢ��v.����ݹ�z����Zσ=W�ܣ�;��dt�}"����^�٠�1�H��c�ʫ�
����@��B.�T�$ڀvǈս����T�_��A(��O����:��f���*7F�~>�1�Gb�@�e��j&Y~�a�{������A��ߥ���E���6�_C��3����5ƻ���d�s��moᙊ���>���#3��5N���57�~]l_� o$B���yS�U��?��${�o�ۗ7Y�[��}�;���mfd�"�c%^��F��lk��B����n�~U�z$�x��{?.��I:�Z#������+�w"�"ţ��Ľ]	o_�T�3ݯ��	M�؛��A�d���<mh��/����]:��|��-��^�_�s���@��#��9��Q���?� آz�<��Sh�߬}g�Nn��; >���-Y�Pẻ8��J�5u|��a�68~l��J|L��O�Aε{e�9�v1c�9��4<����=-;c��U���Z�ė-��1�	�G��m���m�o��Ͽ�?߽��>�s	���9u�j)���y��{���Fj7�Fx��Y뷄p{��͎ݕ��W��+r6�2Y���6��e�A��c��^�ol�uر<{77eE2\�fw�-.��*�o9���r�O:���!����O��o�b�,�kV���]������b]_��媁w��w������-}�������j4R�WJ��$<���l)�k����x����^]r���.�^�~������5|��>�%��_3{�y�~���us�\F��E��K�9�K���(�8�oA�����ǖ�ר>x�9X���rlÛV�M��^�j�C{Y�\��7�:�sM��Fq�F�#���b�8��y�����Ag�(���%ꊚG�C���Ɲ�뻒�e9��I�'����%<"��,"�,+~Ի'û9�C�S�1�>x��p��g����J_����?NG�|��I_5l�ab�"<�9D�����m��{W����}yz��Z����ݩ�bs��م�_, �	-m��z�t��s5������1��\��ii�����bxL��4Y������ސ��=�W�"�eYfg��>m�Cؚ����}��Ӂ+�w\��� `.  GDSCd   �"  (�/�`�!�H �C�F�\Uk�I�GM�����q�Th�\��b�x-_Q���Q���j��/��%D��B 0zp򑿏3.n���)� � � R<'���@��
ew���aoҠ~O�`���x�����#�&���H�e��2Tţ2Y0, ��d�RL"�>XIWn�� �s���ed\S^]����H#�,�I���
�*�$��� �;s�b�pA��\��EǬ�-^�daE�C����S$ӥ̗+������{��{u���}���/�^$�{��{/�?D���k���T���{�[��{���6��J��3}=��*}������W����7�O���3f~������w�g����ʏ�Ca��~$������=~7��������?�π�-�����	g3�Q9ߋ��.(��}����E�����q��0��w1|Ԏ���I�C��n攀�l��iD	g����УՐ �1HHh
����!��J$I��%�����J�ڱ�Zh�΍�6�eim��V[m��V[m��Z[�Җ���Z[Zk���Zk�����V��O���Jꑿ��!?𢡄�����+������9~_m�'���u_����'�EE��Z�o�������W���5��?�[@��><�zVZt@S$���1��!.�+�Tt@���p�N���r�\.��mȑ�4BDڤ�	���p8��v��n�[��i��lF����,,UX�DA��8�$ݑN@���Z�V��$�h�F|�54�A�A
!�+=�肴
ҁ�R�s��y���l�$e``�L&��d2��X,��Đ`0����y��9;�^����*�\.��効`�i��i0O�5�u��2Q##K d�(�Ȩ��i��hNB�$)IJ�� $�BI�> ����"�h&��f��$���{Җܚ1캍��1��꥔% ����g�c}���KG7i:�G���Xc�$��h��]�Q,0��'���Ю����.�����/��}_�+.��`@6(�@ٜ�X]��\
(���{`F�<�(t�k�}1-Os�p`���q^�\�������k�=���-�,�k*�
���r�V��0�u-�-�94���L��`0�)	�#h���I��@栒���*�
�x�L���;e,�T;A��J��]���5��g�%9=�'*�-���%�r��x��f 0�{����,�޼c�jꁢ�e�b}�責䀹�J����"ʮH.�i����/�˚�Vo�L�͌�#exR�B�փ���h\��2��9�th�܂���y�e��R$
��ӡ�s":��6��jK�(��NP�>0�'��C�
9h� /뎁��)�kD!N>��BH�SK �T&R���'G��^��@���
���r���2���y-8�pȆ�P��M�ޙ?"A	FV1@�M�̵h0Üb����������,0��6�@�(Z$`"�l$4��
&@��z��	`���������O�G'0?tϒnn�[q�V�z`��P�L�+��`pt�~��Ep�Y��{�л\k��U��ny#�-��ɭv�^9?�ij��2�Ioɣo/�uW�_����&�@Df<�?��Jp�z{VzH��+�n�C����'Cy���-s?��%޲&+�U�2�E�f^?��{�:�������̇�DH��]�t=�h3�F������r��n�V ����������*quǠϜ]�6;13�������<D_V���;U}�杍�O@ޟ&A$>����(��ޡ�VFg��8��e��]c�#	|.�̸�n���K� �Be܀o4[�6�D�3l���v��M��:�������������E��ٮ� ׈wq��y��r�^o�[\�����Y����Z��N���, ���]��\�)��ٖ�d_A��ZjKp�D����{��������~~7yv�t��??���9�}�7L��ڷ��ς�� � �G8N[ѧ���0���G�b+�?�w��B��o�/~m���t>�C���k�G6@Ɵ"d4�I�f�-�����'���妅83�\�\	����j?9�=e��-#��۩1S����u��}����[�ovΈ{b���<w������9��g�9��p�`H���hq��e��?eo�_�}dK����)�0���Z'm7�m>��8�mR��Օh:�߅~s�����H���X|���|�){(^�~��I`���Y]XՊ�DE����oR�[}z/��h������Y�f[t���ء㌡�qP��Eʠ(         GDSCd   �  (�/�`�-% F"uI��*����Ե��(l%+p+A&��jz���y���S�U�g3]z&��X-@x�����dU�.��7����oI
W i S A!T��/���Y�A��W6:A�����+/t6@8���O����I�v�g�GD旅��~��hw{vW��>(vw���;��-~��������O�w����o���p�r"4Y����&08���D��"�Nn1Rd'&��	@a��n�*��bS��l+)9�K.�ŁJ���gҕ$zt��s��8��V7�$�~���"%O��	e*�r\��f�T/G$����T H)H)�s�9��(�t~�+]����$?������(��������~����?�D�:*7fx��ȣ�\b	I9�-�%��7Ъ�lGC�rV�Y���1&�b⋄��S�)|ux�\�R�c�.ގE�Q7*΅�Έ-�U��Ѩ������G�R�B��� 8�ċ���{QFIRR�J�DYi�`'@�L�(JI��9�Ð%�{������7�Rwi�X>5+b9�֜I͢�q��	q[P�ۄ`��T��#���!�����Y�<]BO&�c�Cƫ ����֝������S�l���=�jp����Fg��zf�ax��X�D�9�)a�~h��֔��U�e�n��~�cϱO��fF2�����[2q8<[�!�؏�zK=�kik���>�C}j���ms�d:%��:�獲|i�J�5�9�s���9)d�a��4�8�:e_��)�bH{��-�{{�{:p�s:����ņ����ƜL3�c�!p�V�� ��u��͇u�93\�����c��o��{6���<����n�_��+�N��'eчZ*�К3�h�{^�����'��`81 ���d}[l�'�B�T������gq��������;I��3�Oȷvr����w�y�m4di�V�?0?�Z�i"�h��#Oov8*�7�g<z�k�R{<���4f��9�xS~D%���Oy�4�}g�a>�^][�n�:�c\��bs�NXζz�[�G��"�A�C%7��I��jmqBCx�$m�&����{щ�\�1�6�8���s"㷽�Ý&������̍3���]Fl/�7Z�ϝ���>�����{��'�����0�8����%&}�]𤧫�#���5ke��N��h     RSCC      pD  �  �  9  �    (�/�` �> 
U�I�qR)�:73qo��ϧqDM�y�� f��su�����u־l��+�'�m~*��r���U�k���qA/m"S� �-�����E��ݲ���eo�V��=�}�ڨ�M��N���w7��6.�,��m�EzO#VU�V���U�'}o��Sڹg���=ѽ���T��j�,�բ�����]S}��Y[St-bݳ��ʦp������o��ۅm�t��"�����+[[�W��N�U�?8���F�-��s�2���m��Mo熵��]���kX����b�e��i�/��M�~�{��,|�zƪ瞹E���_��=���[M;�+���k��/T�ˉ_��x|	��a�pd�?�� ����B�d�����/ۦ�?����S����o�"��ۖ�;�D����O�#�f{������f���0�Ϗ&P�m5�޿R��4$�"���8,0ƴ�6�� ���R�
.!�����P��u�~5�Ж�����6ug ��+�Yj�w�h�(�]\?>�~i4��k��W�l-�Z�7�Y�ͷ,<���o.K{W��~ݨ��n���3MS��a���˲�S��N��ڶ�����7��g�"�k��ͫ����wh�.+y;d^��H��efX)�*�,S��������s]Ks�K1��#�+�L��B�����y'9�e�C�̱2�o�!���"�Xi�f��o������9�bge�e������3���d��b��R�/L`�F4�h*�t�	m�5���xj߈u�F�$���I�$~C���2�XC@c���y��I?>�3|�@��z�b�B�*����<:<x�4����xa���3�p��#�L����O�D^�y��|��C?��	��D��o~�g�΋o �3��F����L ^w��P��x����.�>�\���O}MO��s"ЂJ[�3�H�������4<��V���[��_��Iу�t��:����&�+y���H�k�d��0�G����iz0���p�8�z�����p���u9'�y��9O���Г*����^������]g��>��w�{n�*oE~��?���_(ܗ�;P��:���L�廾�����P��{��=�u�S*��[9����)CB#""�$)Q�Q�B�� �5I!���$���9�X>d�%G:�]쓓tA�i�qM�c�ʀ�.8�/�{A�b�}�L�6`���d^ѵ�2��hz�jJ�0	�Zix[e�ӹ�@�M��b�2�Nww`9o�0���<��Д-�M�\��Yǹ�Hp�xw���4�����5�ď"h�`���%AAG���U��J���P�fg��� �P0���zl��B]�� T��P4��N�o#Q�iO��T��1�j_s�sG�̀��O�r���q�"_Qb�����vΉ��#p��rĝ�XY���kEVX�X�E�Zp^9���_�T���Α폊@��8`C\>����_`s�̆ذY�z��$e�,�U6|���h[!r���G���Q��GǦA�u<�SK�/{ ���b��)�E��Ht�m���e���M����)v���A ,Aɯ
��]���}]���R�yI�i�G\��xv�DЯ��ƛ;�^�yCq��&�%�BP+��;Jc�"L5�$"��ӑB	�G�|���F�Z>��!(\��P�����
ڔ3�"r�[<*e�3AݗȉR����o�9�'^�X�*^h��Vޞ���E!<��+���9�k��]tnM�zR�"�ʈz"��j�B�3Ld�j�=�Zf��qn��p׬���gE�;Mp��Ij�V�����{=�7t��P	�9��?��%�#� k�zG�M����w�Z=+V����6�k�X���}��C���󈝗�i7!��_@g݂hx�0!9V�z��	l�>aG���p�b)N�%�8$S�{i�� Ǭ�F�OA�K O���N�O�����(�/�` M. ��x0��I����nw{�i�J]�Ai	i�e��-�b-X��N���[���[�f k p 73]�mr`������,I��l)�t������eAI�����&���ӶM��Q��O��7Q J�&b�IC�P���ln���V9�l�K���Y6B�6�rY�%�3�8 }���-I�㶍��������ܳ�j�8Da�U:��E�N!GO�ʍ�@zf6{wO�70hި�7��L�K!��f�N���e�Z���I3��T�8B#H���m.��"ja��b9�Dݼ$�h��v7;�"�$x���3�<U.��L�LԽ0�@�0�t`�&�|@A��"3+u�E��w6�bb�=JuPOP`��'�����ܬI%�骂��C7/vIjm�kY�!�>�6���R����80%���J�[�X_�k���j��g=��Ջ�KҗӦ�g����Ş�����w�(�$`A��l6������I�l�Qb�T�13С��$��>�Y���9��%ƽWf2���������������;MY��Zwtl�e�$u��j���C�`'��p���a	-�f3�,��4Ѡ4��/�2
7M�m"($0fNA�,mZ�O���{֡F�Xg�Ͷo
�¦���lh!��}S-�%nI�-~��kł!�X��M=V��(��M���":u���п\S�߿g��EA�r� ��?c��c՞u?D9A��O�<M��!hD� 6<��C�#�?s�p�݇?�G�{5��)rqǝ��b?~�>�gC���x�l^�b�Nٷ���J�r8����<��ۋ��sx�2f0�(x�k�$m2�K�q��U:��퇈��TjivlBd�Q�6[99�fXg��(n/��pN��Fs-�q��H�0ێ݈���	����k"�h�����%[6��F7V� ~�
�}�&B�r�"ʚi^��e��Ug�P��43����L��c7 ]\=t0%at|�|�U�֨��i��[Q�;��i�Z��Ks�^���{� ��6?��! �>��O���i�DD%�]ZV�� -|4Gx
�ݑx6����0��%zbI���O����3������W%E�C�v��^��*C#91t���|��0�TN��As,����L@�\b�������l|�5�C��S��P,�<����lו��q���#G�ؾ�R=�E�� �)�\Z�Ha��DjF�����9,�&N�RZx�\��r�|�W��R�GM����4ÍA��S��GX��:�rŀ�RƮu�I�i8�l
4��&�����>?��1���n�b]�#Na-i�`��aR�[e�"�a����5�9�E��Qfp̲��bC���^�h�H9��hc�eLPJя��E�y�j��8��d�����x3v��mKR��6�~2�W�p�?bn
���~�-�=+�
.J�°�?[�?�S�V(�/�` }1 ���0�5I�f�������;���3���$6do�����t�����УO��R�t y ~ ͜ڦ����l�f�/�Op�(�"{C�4�n�L4�Rfffè�>�rU{,]��\�ㇸ��P���GH8�vL�9��͝ڪ�r�[�Am��ox�r_o(5 ������%w7���x�.A�t�6�Ƨ-�e�!�%KPp�\��;�� �ۑtG�����3G������vŗmY�I?73�j�1�c�
g��ĎI���^��R���צ��J�+�YE����2�67{�iL8�Q�����7����?|o�������0�PQ�� vV��Ҷ`yC᧳:2���I�,e���c�Q5�#�I���;�cL�q�4 �����؃/�e��8Ù0�g�_:\�`�Ce�/��ܬT�#�(����Q�v�� �6��*�Y��R�mƟć��Ҟ�"�5�j.��
 ��)�I�fD��1�(vL�����"�\�������"�{ɶ���,^��B����ѭ24""�$I�91!����5ǣ�I�$�k�}��r(��� =^vXo�#�i>ίU��C}�A]��U8(��ѷ�T��Ս*�� ��t\���l���8�e��Jd�-;GTB��������c�}D­��+������M������wכb>��6�o�5����=%������?7p��,���!\��<�1�j���!2��r�ڕ�i��!��m����7Q�%��Z{���X��	2�v3"��^��n��;����1�T��<W�@�7�;�X�L��q��}(��7����m��
R8��k�}��Xa�p���> ������e<�0�y�U�o䠻��r���ը,����.Lj՛�ܳB0�����Xi؆>���7;���u� 앎� >�^t���^X*o��19�;L�/ԝ��Ε���-�l�2:��-��#��?����fDG�Ìj�@Q]^������1pY��̸����I�, ��w��9$�_)�aZ5�ǰ�n��O�u)��0W���ėjS�mR�ny��nW�-�)�e5/�F��\U_scz)o��W�,T������x\���{����?��!�a�{$�Rha�TL~��j��;6-t�+aW�-i��;=	�X�����m�߬��ؑ'�#f��a��:�O��@�eP�Z�=^�9y����4��H�i́}s
p�ğ����5�}����}00�
FYB���y�c�p$���^@Ōe��o�<"�1�iR�f�:�t����hH����Fn���-$�Z_��eC*����n�ܬ�]�
�+���P#Pn_�g2-aR���<0�Hy�RB����Ċ[0^z�'>��iW��V�~�0�D��ѿ�̑|���n%�|'"�owƱ������v������bIiK�2���i��rG����3�������?�.ʣ���f@�@�]r�#r���#x�ice>�i5k���h nU��i��
]��z�,8�*@(�/�` m5 �CXH��(�${Iĩ�� !���(
ؔ�2 @$�0d�w�C0� TTj�ED aB�B�j
���.����[k��e
� � � �J2x��;7�x8��� �)x+)�j�PƹGM��{���T����ʟg���M��������;�
D��?.��wǦYq��^�6zk�m�b�M�6�a���ǦUR�`�>�j���y�f���2a�q�U��mR\��g~��5+���{�u���1�:��H����˄�g�]8v�՟��κr�����8���\�:�+-��sΐ8v�'0�KI^�Q̑��&'��v��y�8iV��K.g��F��yÎ��.lRE۠���ަakvaGv4�n���ڦ��;^�V��>/%��Z�u�P����I �?]d��,g9m�	��s����8�|�3ν�}�h���hk����Y�����`��Ii_�w��
�p�:Z Z;��-_����i�l>!V�V��ai`Ű����P��L+)�$�$**�*�UJR0R:)T�
j����AY9!9�8U8�~�b"b�a�`2`��!�jx�o�SyYJPJ=J0JJ�R�Ri�(Ȩg�`4$j"��vD2Q�t:���₸	�;g�֨(j�����:��Pt�P&��a��ִ�T:��t�3�C㴁��sx��l���*#�y�[�6���fݜuɄ�"�BN�XQ��͍�͌�
>)t"D��!1�D) ��;�b4�Q~2=��W�^�0U'�|$�_���w^�unI/x�Ѳ���LѲ����O�}� }�a��Q~2��{?�y��G'�>b�_4�wt�js�運>_g����3�;8�&1����Nl&Fv���Fk�������h�Z�v^�W��+���e��㒹L�.։bs�ք�`w��"�2v�*-'-#������!CS#"IR�J��F!'%� D-N�
��L�%����f/�K�Z������|��iK�l<�g���Wؚ��{�;Q�5Dv`��?�h��1����ܲ��y������N������.i����k�����E����K��?p��+��ߨ����t����� ���7#�`�-���/����?�Tf;�1'���|�]ȕ��/?�� ������~p�
�Vh��������:�7�cpڠ����с���>���P�M�1���䙆*��{�W���$��2͗��w'�c;ny��RQ@�޲hE���E��ǋ۴���S<+�D��4��2�F?0U��%��?$�!���){�ͺ�I�����.#%Z����m��|�\&}�ҽ𐼡-�������\����$�	�x^�.E���Kf��Պ]�|�xl�,�;P�����$�|�D�<���D���,�rxc�~�`� ��^뜼��^�a��ǉ_�q ��9r+.���y������9�k>��YD�wR�881�Ro��[��Mz��=J4���\�!�D�cЊC��n�����o�+�|�.���V`A�8:&���ӷ����Y����>z�3�|�b�?"")�Z�k�)�˅�a����n���u~<tO}�2f���[��!=B���c1s�ױD&�@ɞB�_ɻ�3U�H�L1�!9`_3oES��yNi��7�JyR����~#-�~���ES���1�!b���><XI{<�������1ʊ�j(�/�`p% �Z0 k�  ��ok#�m#݆E��3�>��dA0�#b��|�g A�PD�RW K A �Ԡ)���訹��a�K���$E�ѽ��PH����)�(ZmC%�G$[hJ	������&֨$!5B]�
�L-h#�$md�g��[���֟�jw|��43����?֫'C�疧��y�%҅kB/�tV�R<%T"�"�����hr�I�E�y0J��{츛zJ?�7�_�2":��k�8th���o��MY��o�=�Kf��텩�+��f]Z���:��N�!��<��`�
 � D������~W8��X�`�:�c��4�a(K���l=�~���X���t�r�2y���.7��? @B���(��=��NO2��]f���p��t4��(��/�y�6?D���d�']�&~� ���6�����s�v���1J���܅��W�+�`Ԓ���i��ϲB�o�d��;;֘�\q`y����O��yC��ӽ���[&5��0RSCC     [remap]

importer="font_data_dynamic"
type="FontFile"
uid="uid://7am1h57ldd6"
path="res://.godot/imported/PixelatedElegance.ttf-aac00801d681e5d2b42b23257b2692a7.fontdata"
     RSCC      �� ,    T	  q  =  �	  
  ^      �
  G
  x	  d	  T
  �
  �	  �
  �
    �
  R	  �	  
  �
  a
  C
  �  �  <  T  �  x  �    r  j  v    �	  �  �  (�/�` Y Jļ&O0֪Ic����?��;�R	&��/`'_��~�w78��s�pϫi��r��AL6����BY���S�6�0	�{j��+![�n)vHOF$T��,�԰��T!h�o���g�|I��#:n�0<}B��nЙ����$� Q	�ۺ�p��!����+�����A��
�r:~�쑀q@������d�%Ϟ@����>��K`ӆk��˹�#��#�gJ���d�G�m<��]� �~|����|VS?���a� ��*�����qPi�|7^��#�ZPa��DqC�[^t,��jU�H%`�T�̓S�H�����.d�CԓT�L�r0��]���ؘ���,���Jr���R�r�c%������i�S�ZY�6��ۙ�[U�fwe��`�
`�,o>�м�i��~A�{&nwV��-�Yެl4����;��v��nYLMLf���?��&N���?��ʹ��>0���;�M�ɳ��[	���-]���w�̬��3�=]�b�N+�� mZ�nW�9�xջ�lV8 �3uV6��݂@Ά3{W:�mg�z�\��x�&����̟������Ο�g�^z�.��>��������ΕA��]�L-�;0A�#�;�z:�M��邽�?#����Lά�6sWu�n��x>�fo?�g^�A߂7�_z)oU���ꇱɻ�hl��h;��ݭ<w6��a�@����x���;]T�wF�/�q?�Y N�B0�:4��5�Z].�@ɱ�B���譌=W�gsj�)k>b�sTkh�H��k���f-D('Y;C&4w��Jۄ�ru����m*�6蔉�\���ݵ\�Pq��Gg��;kf'�t�v��e5[�k��f���q�,�z�g%�Le2m�^$�b▵F*0��P�<��#�Y�V��^��>�h�S��ڴ%��r+f#��tŬ[�i^#Z9j�2�����R;�VS�q�r��F��TX��H��C�,��D-7Qgca-�ў���j�Y�a�|�Q�H�<jEQ�H4���=��z��n��='�O;� �@e��#��}.��Fh��,݊-���p$GR�?U��X�-���f�p�pJ
����[��U��9u��*�j�s�p ��I�p�J^U�܃����u��8;d�5�Z�5je������Z"_����km��1*�)�� �c!3�5:Q�<����K~"��VmKQ����wX�o(��]�p_ �'�4�Ȃ ;i
FZ}�cC�(Q�"HZʐ���#"�H�_���Yq��
��ס\t�iT��!]4��y�2�ؑ���μ0b�)L�~h�"���lm#��P��%��)�j�Y�,72�oD�F�Ө��D<� �&�c���}�����F0�X4��4���Jj�i95�$uP�`���X�K�a�}��f����XGe�c�����&�*�l�`��:⎃Xa��
'aF��Ʈ�Ԣ�T�i��7˵����Đ�h毚;���R���Y���1��{�D_٢��*��[捻���2�5A�;U���2�s0
ǳ\?�][QyE�f��v-��[��eυ+|Vϋԭ����o
�9c���oz`����ZYj���n�Qk8d-h����J�k�L�0����e�2؁�}*o.F1�#�!��l��T�_���|b���݌Zc@"1���?#�}F�=���c0kf1�{�p��%u7�����ޞ�٧��H�nc,��.�s55�c#�Ϩ�YD�g��l�ϰ�e����T�e�k.���F`�u2�n*�Bc�ьHϨJ�P��L�-F�X��Y�X��/�]o͔Κ]Y�-V��`�b8�b��N���4u�Z�/o٥}l�8�d�������\5�@}���k�Owm�D��!��µ�K���J��i���-��!7ա�3�2�!ZYH�����}l�3�$w�Ok�Oo�OG���-��m�<��N��Nv�L( -����_䯽�04�\���ٌY�#Kr���.�����R-�6��=�j=��_�N��Ke �b4j1
��Z�vM.Ԑ�2���赌�)l9oiX�e�v�T�,�]#P�6R�lgņH�<	��(�5�8�9�FF��B�M�Q��j9����҆���v�%�i,?a��aOb�C)K;iL��Z^T�mآ�aLd���.��a���Zn#�W�>a��T�{�c"�\��H���lѾ1��r�����-$�M�v�M�Nu2�e���=u�ږ�Y'�i�P΃8>G��\��#l��ͺMZ	���3���X�gi[���<k��d�~`%���h�*���Ё%�S+m���i��c�V�(+��J+u=s%rղ�ٕ�fEb<z��ȣ�ء���Gj��Ȳ���9EZh�lc�g�=��Q�m3"{-���[�lA�r��`?��S�o?�q��Q�>���
��
d9�i���V�"�㱊�=�b�1�(\�ٚ�w\f���2�(d1��7�����b���5�׀��K{怛�3B���I "5u0�2*n �ÄhR`�Ɛ�K�Ś�w3�����G�^īZן!�k�pw��w�#���z��/��D�D�쏪�4Ƹ7�)��!���� ;I���P(g��6�bb��3����c
�q��r�[�Dz)I��A���X��~�
s�HUgC��5Us�S���⤌�*,+�ʆ����2�������rm:�+���eЛn�,�;g�G��T8&�ͩM \�������t)�@�Y׋�0'��:ܼvQ(�4�Ȱ�t�`('նk,���F9d�$��9�Ri-� n��p��#��N_ڤ�_��!�񮠫�]QSw���fO�*t��T�Ҩ.(�/�` �7 T8@�lI:����Y}��DU?�䧗 7�4��v1L�9�4�:�y�o����	>=�����[�� 	� �Z��^��i�YS�YS!eN��u�+�v1�;��1k
d�T�5=2k���x��9ǖ�O�Y��׮��}L�wȧX��I�=��i�C;�+X7�#W7��JLd�X�+⢂�K>�W������L�ڪWX#o��K�����6�3sJf�T��i�iS����R@|Ȳ8�� ������}q��-���$ӦK�M�n�)Ӧ"��Oh���-֛����S,3�F��/3��6�䧘�ܢL?�5���'����tsj�T�lf63�b���s�f��>3@/����d�b]��Q�ĕ�m�͘P�f֘O۸��I�x�Fq��q!a9���_+u��#)�;�A����ɣ#�x���A�0���DZ�;�b��W�N��1=(�K��C��Gx��u2h՗��ե"�\���aL?�:)[G�Ỗ��.�����,�ig16������m�.+�,6�d�_��4���8��^���̘d�cb��]ØH��.�TvRʮ��mK_�֗�`K	�nI�����lɑ�.�r�掭������[)�7j܎1�d^���̱�Ff��U��n���7�n���o�Y#��7F�%sT��@�{��nӱw*�&Z�f��e�t[�I�ß6�Jw=ev�(�j��9I�S�%[w��z�Լ�_v=67�l��%�#��vI�g�mҎ�k�)���{��-�E�ƶx��f�t%���lj���	�޵fԭ,��٭[Y����6�Ŷ�J}�ML��,�I$İ� �"c����4zF�h
��J:=B^ωG����<^ʣ�A�xG^��}�����<̯|�M�u ��X�w���އ�O�O��w3��#��[#G��c���]�#++�U�j���R�yw���76�1���e� ��?� �'���ݟ�}�#�rwUUW��=O#� o���@��e�6ز�G�^�7�%��'?n���������$�|��F�FAR����)Š�>`V��!D"3��&e$�+�Df3ȑ؝5�%�LS�c�_xHÿg�a��d� 6�vH�s#Q�0G�|PZ5��q�(<P�3�7��c��`@�}1�����d�e���X+�+���Q%,�"WӔ�8{��09<O�L3O�+�27�������Fq��h1#��Q?pg�"��8JI�{��!�JODj&�.t���v��?� Avcs�p�[�5<Ľ
��C�,�8����E��w���0��!�W��~��-y����ϯ��i��`�BI���l�%r���a#I���/pu���U=�]'-�+4��MW\'��3�ჯ����|"���Is}�N�F�`T�ʎ�H�Hs�q��-�~{oqT��{(�p=�}�8޽�P���й���w��'��2N���+Sg�1p�@��r���t��o�k�O�Q�M�}�F㇉��\���^�����J�Z��?��7(���;&�']ii��~=���z{U1�OPX��﫹�Yt���-�$>x"��,�b�{�p�����k�g�>vn��}�/�<#��\?�`��t��-?��_H���r�����|v�ۛ?��V;V��2��h�c~&��+�-?vc#E��	���Jˑ�+H?���A�[�MK�3��8�����?׶ራ�$��fw��ry$zQ�#�����-�SF�
�1���`{���&�;� �-��8H~K��)(�/�` UJ ���B X�cP��pҀ�7Z�H��U�	�����������Q��{��[S/q���[�h�R�:���/�%�$��2���X�%Xjɯ|�Yke���Y9%�rO"JG������ϒ�$_H�U~ �z��ׂL���Lp&=�m�_�!B>Yǅ��9T���#��age'��ۍ٥iT����$m�N+�+,mL�ZZ�4Ki�)�k�U�Q+��G4���ƃZj�D�Ԣ�AY�I�ڐ5$�����꽎ރ�=�/��g�ލ۟�}(�7�F����zo����!�;�c�������Á"���v	)�}z�� ��K�_����3�<�O��B�����z�7ŧ/	��aʈ���g�5�-�����]}V�����>�/���� ���YnJ��ܹ)\���Ѣ����n�lɶk�쒭a�,	{�9�M��T�����Q1͢^��8t��P�uS�e���v�H�H��!$�0�B,�� �:0�@)�Ͷ���z^���L����^���m(>myI��:$�t;t9���]�TEP��JWU@U���w��}i��/��c��i�e�M�]}|�/��M}0��@�6�2�r�0G���!�Ӫ��-�gk�h��rm�U�Iv�a%X�M��?W��П����������y�5�T������D$ٿ���eF��P���X�k�`.A%xe�����#(ƀF�=`�쓅������n����-��m�, �B0�Pl�N�I�l���) �ZY�ݤ���b3)�n��^�>�� 1�%� '���,��B	0� �)��PVRKJI'�4 m`�B�Ь}�  �H%�!u�8\�Bi!%���41
����Ӂ�m(�v��rzӚδeӗ�4�'�G���F?�z4���;jG�(���b�[1]�:Gі����k
G]�T�Fʹ���)^��V���Є>���bA���$��K��g3���Q2%�ctL���t>B�8���9Q9{�8wQ.
�-*�oS0բn6��R+�f.e�g� 
C�4�@�J�[)%�$���)�RC�,6/�2Tn���S���l�N�)7��̔Z��-1奴���RNJ-2�Y����k	)�CI���$�'ߤ�4�6Y�]��5�&�dY�V�L�ɱ�Gd��Z+*�3�� �k*c��ا⇱�b��~ ���)��5����}5���4�"�?TH_�>�?�>�T_O�sGߎ~��Ι~���>��}'^B	�$��l��������y̘�}8F�����k����E��ߢ/��E?L?��^���o�.���R:C��_藭�L�7%�|�|��ɝ���H���G� ���ȏ�7 �F���[����-�)~|6�_������/|.c���~���qп?	~}���πO_��/�S~���~�O�ϔo~�_?��k��>�Y� �Q�#�o�|(� ��9���[���������?��_y�����O���,����[a���K��ڟ�k�����%�ѿ�������D���IJ��������X�u��W��}_�{�%��,�����{�����>���_|?�%���E��r�)���L�'���[����4�r�A��c�Ͻ|*�������=�'�Xč�������oj������}G�j�i��+�_����{�W�oh��B�*��礿g_�~��<�x�ξ�}�=�w�f�m�5��b?�>�~٢�'���'�?l?����-�T������7b�׾��.|�}'���ɾ%%B�}!�k����~�>�/ُ�W�_�~V�} �i؟a��KR6|#�E�	鿰���CP�~d?���I��=-� ���w���`7��`ߏb��[��G+؟�S��c��^��9����.��cu��3���7zcطⷱ_����ѧڧ�S�r�ߨ��DD$���$I�$"2E@&Ԩ�Ͱ��I���%�������)'n�̿�y�r�{v����~x��\�i'Ã�ѐ�0��5넿 �l���x^��f�#�9�N5�`�+��fn���Ƚ��*`
QOa�i~p��:�7�:�AF�{�g��Z�g���n��~=��T'��$u�F�ߴ��Wv/]6�!��0�䕝	��]$t�'���������f�A�������f�rW8p�!��ۊ_h�z�{��~�5=��'���|��q%t�<֐/�z|�c/Yq;���F?�s�8{W�W��E�5����;~F5��q.�d<�S���a����i&&?-|U�����bH�>(�RUa���ᘤ�"gB���Q�
-J�R-01���d���
�s5(�/�` =s 
�7A��B����v��;���?>��M�y
�tA� t��,���8ǟ��_��;+1�mޠƛ�)���L�+:a:?:Er�r��h��━����<h|c�&�l���ʳ���y�t��R�iN*�SLs�i�ܒ?�]^��o>�M�o�O%O�B7A�����3蝞;?u�Ϥ�^j����8�������E�����#����:��Y�c���(�uuk�BE�Bc]�\ȧ{vvT�&��؈n�<���5˃8t��2���U�,�ٻfy�R��$�O��p^�ln��� ��jK�z!�G��u�"B���*2�qp��ͦ��F��H�:e�]�p�GAQ����'5w��'��ĳ_��Jg���ٯr�Z��A����Ǧ�ڬ�.ݒU:���-��8z_�|�=mV�ΖC�Zy��Tu���W)�n�ٯ��T���fYYs�kv;�n��iU:�����U핎\O�ȹC�z��}��6i[��}Zmk��.�ڵY8��f�B�^�������N��﮽��R�t*�f�viVǮ%��:r�f���n�`�Ч�m��_����-�>Ֆ�]���]�n�ZT�XF�l�}Jo�j���mU�5e�ܧt�s�N����ܯ^��}!���|�ru9s���kr��<�j�rv+������5����k��������<�/T�_/�o^���]rC��-��?�_K�E��˩�$�?7'�����/���G��w����7���I����sM����U�	��?��Sɩ��)���?]�`�/%���C����?��I�ϧ����r9W�w����hO,������OE�G����A�R���|M�?������Y���|����'��I�_;�)�qsq��p���-#���ù�]��EX�CD
�1"��ߐ�Ux32bА+n^nY7 ��+BN�
	b#�P���m�0�x�� b�[�6l�?�~����ǒ�,|0���8z�фG�;<����@G��֚�����Z����b���c\p��a�;M1/�-�,�'�{Czc� *x`����hY��� S#��3$�ɲĲ�2�2e{4�h��q��	e���v���� A�%�!���q��k2F66�{�d���2�d4��E ,ƈ�&UO^hxw��삋�(.�\P��;{!�B��Xala8����-B�`�ŁM�����b!����k髎�^+~Vz�y�ym��^ܮ]W]ٮ]֮G.\)��\|*V�X E;8ňi[-�Fl	m5�E���e���bخiQp�@��a'0�f�]"�D_�5�e��&!���W\!���A'BL�SďA!��@A�ʲ�0�S _�䪺��Un�T��|��A�>��� L�I}I=If���lF;�u`�!1�-�**e�J��A�C�K6t���p  Z�H<�5�i8Ґŉ�	�I��I�P2��P�,�*�.���ĂW*�UHC��LRL8L�K?��Ka����(Q��tU�QJc�s��aX%,�L2!k�	�	h����5�z�b��r$)H�"�@�Kh!G�6"\#@B�����(�ўQ F:,xXŪU�!�O�$j�jC�2}���>M�a�LO�h�ȫ�A�NQQ����"bd5`j@�ΐ>�S�hUR�$�9b��BA����B�T, Z� ���W	*$(T%(F�QP�@n@��[Ѩ(��G�?�'?`?~*}�����d����(�q 7��3%vw�v���Lt4�F��f�!:݇�C7AAA�@�@o���VhhhBƝ���1��###�s��x�������vF;���g����\�l=?878�y��sf3����m4���Q�q�6���̼�<�|5/0��n�Ȣڢ�E�S�N�����e0es\斟�����`e��T[&*� O��d>rY���LFޒ=���:9�\FqR�L�K1�"�#��NE1��D�Z���k"5�'1���xq�e�e�d�cc�5�3�����J�2bq�
"D���Q�K�=������!�C#�j�b�W�M��Ý�5�o�E�>C:���B/�x�L
�
�
�
�
):(�)\#���d��8O�D�C|C<C�B|B%� ^ �c:"~	����I:J "|B�A� �`�`� ܠ���3��!��B3�_�㰗𖰔p��[-��
/����0A��X�O��`A�1Mn�
J���'���T�q�`3�
�p0�F�e�/��[��]�������F}���8�3������v�s�@ � S � � (P�ҋ���ݽK�l^4o�7���E�"�B�c���?,J�������������z�������~\�����������m���#�����Ƈ�/�b���)��.�.�.i.h\|� n��F��%-N-wyUyQy�&����;���-w�ݻn�.�� ]��S����>q�(����,P�Te!�������r�u��.����;��؎�����X`i_irv�_]�:3V�dY�RX�G�*h*P�b6�:�f�:":2Rޤ$IyG	�b	%����#O*<Q�[!�@щ��#'CNޚ85�d�I��%fK�(!@I��"%�IҒ8%Q ��H$ yA	09��j�������֌�q�M��o������k�~�nrr�X箕������ܛ��?>������w�|����j�q5��Ќf�h�H��c`�I����Q�l~����\�� �|J�9�-�Sg��s�N���=�Oso�~�@�	(�M�.[go�~���&����M�	(��gw���R��Z�߷��\)��k�~���y����w�n���6�O������ S��1�\�/ۗ�������%��#�n�8.�.�.�.ǈY"�FL�xq1q0�v�1>���xq*q�x\|H�F�+����g�Oq��P�'�g����p7|7���>x\
�	� �x|�G��s�S�C�K�>p'��%ܒ��kI�r��K�h�0,�,AX:YR-X�X�y#��V������{ý}�o���Zo��o�7Л���J��n�J�0*�*�*��:�e(ٔ��z�;��9�$�D(�(uP@9�R����0O�3
L�˗_�/h�bܒnFw����uOq+q�p��G�p'����Pw�$����.�s�@&�&�&��L��d�$�d�c$Ւ��v�G$�$�$�$�$�$��b"#��T"�"mDzERE^�,�D	�̄ĄlCBr!� � � � 3@N!s xT?>v��<�xD{L���������r{����&k�hoh�}�gؑl1��V���>c�`�`�`K�-���D���ڽn\�����N�pkk�5�:ƺk]bM�fZK�?��zu\-�N���.�9u5�j�_j���V��QߨդH�I�Ӊip�KZ.�#��FKw���Lii�'�L�qT;.:�9fq�����c�c�����%�!G���Ftcb#�����Ȥ�H#[��F�F�Fj�NN�1<c��%"#1caAAZ@!n�m��b��@UU)Ka\�c�=(ֽJ'����9�`�M���+�"[hZ85����P��J`� �IVx�R/!閲�{m�9\�ҧ!*�b3�X��+(�/�` �Y Z��$L R���Yy �C��x8��"#���<�dP��m\�ً�����bwd��7d����wh��-5&ʴ���R�B@2`�H��B��U�ZWh1a�dY3�L+p���,F�c֚5,WȠ�VE�1�V0`��X�B����+V�z�^x�P����ǩ��3F9ўϷN�T<�p�U�R�*�n���I�P?W��ܖH+����\���.���[�ve��@eiU�|��
�r��$�͉�|[���=	EI!�7pxsJ�"��ǗI%KZ��6�� $,?p@y"[af���K�9+¸��¡@��B�:-̺��Z�q����ƨ(cЗ�=����N�(*�o]}E#�He4�z�P�U�(^�V�J����DD�WIB�DD@t�H�-Q�N�
 ���
ׂ"M �_��K?�d��5�_[z2\����m��x���z�����F������L�琅&�M8�+�`�W�E�1�� S�V]+מVGh�h��ie�UZij�Bl���N�MjMh����x����\�U2�.�b�e[�+��e[#�%õ+���<��v�&g]���n�8_�"��0|AA�gQ�Of�Lu�3~۬]9B����銈�7�sn��7��±�@��0;D(�y.�,-..-.T����r
�\XI������O�b��#G:t�H������1#� �8�.�	����x7��}ǉ�����w��z2��=L�������a���A�)t�t�����97���J��9x#��n3��~���ֲ����F��ۜv֋���df�#~�ӛ�\h�x����W����l���z�g�f�xa�)��A�b>�PO&�9]B��I��J�e[��>��f��0l[�\x�7:=/��j���T��ДY��0i+.($����V��F��(L1����j�����-eV[�n�a_�#`��.��D4eE_�dYq��&hB����N��0�C�v��[�]���s�1�j�	�=v�Ac��'z�������#PCHVT��؎"6�l�ՙC��8�gWI�g'e�65����`d�L���j�`a��	�o�N�p���
Haۙ��CeZ�3tj��bF�xD�zy�����SPONAT��j�x�i���Usŉ�M�U;]Ni�V�tkN5Z]�]s.���*��M�C��%o;Q�oK��ƙi#�ZZW6��4�� ���+��9o�oK!�]�HZV�x��s�f���H8�P,^rQ��I���ӛ�@JޔI�<�7�����QhϷ�*3���å�w孮z����ۦG����ѻl��;�M�O���߁'�S�ϰx7��'̃��=}L8�-FL���|a�G��*���Z�j�o澆9�볇鎴�5f�
~ �Y�S�AH�<D̜iy��"��[|h�H���vx�a3q�	�;D���m�)�Fƅ��T�'����9U|���UY$��>��������������&�z763�W
[?�G��>���˶,�˶H ��8
o;���҅����9��<QȤ �A�K��s���)eD�Q��m�H��W��2����#�x|�s�7�:��$Oru.��X%���%���2�g�	$�e6y�1�ms�x�@�0�7��y��.ۺޒq�l{Z9��	��c��Nx��1�1�tg��WO|��z�_�i�]����3���p��4����;�Su;���᫮�S�����f��yۉ4��
�	�$L���a�u�x<S�)He�X>?:�/�.i5M���|s����gف���1��-pE�
q+`��^uj�]���ny�/y�?�%�Cu�����6ke�m�]�g�!�f�O����m����p2����\�{��N%�\X��Q�a�e*~�w�;��~1Z�\�_f�W�U�\�-�vԋy��y���y��m��/��E�/_Ԕ�Լ�	�����[�ac�2�ʞ�#����v�(S2��wA�mo�M�*z��a��,�F�q*?!����3b� �ġ���.�]5Z�N�˭;�mwʜVק���p'�yۙ��a�#,lǖ�"9�`�����*��ɓ&,q��	>{�0)S$�	Sar���J G�GȾ�dq��95�C3���-,�d���N<q�=}BI�HJ)+�I,5Hʍ-���H!ak�z9���m/J��՞��
w�5�zc�P��i�)��;N�h|?���>_����gQҀ����qG�{0R�.?i8���o� 5g�E�س����2:l?i{��a�M�4�m���KQ��|�Qu��h���!���W������A�ᨑ$Ɯ$3����$I��)*`-SI���M ��E@�%7�6��42��శ�s��n)lr���52�&��C�{��(���|��h��0�Cq_�^5;�i�i<���H���"$r�h=��	~����ؐyR8��J�'�h��@�H�s+ey���2&)�o+kW��lЍ���֕���7,7�,&��梚��(������PJhF�U#����!؊0K�̓�?�r�h�:��y��W�@��Rm�gVl��TЄl�ޚ��m7���-�rЄ�q��X��1��<�f�����ʃ$�i��Y\
�N�N�Vto\�ĕ3���.���藋󳀽hAN�ue�����?�M������SS0����b���R�p���=���pp�N ��a���������	��4���1�c1<$�Z�X�*�� ���2����9{0�Jr�K!�7	g��Iٝ�L�(�CT�<%����i�
(�/�` �M ��M ���a����<�Iw/1~{�U)���9��H|��ws	��k�ہ�u�v�U��:-,����LӒ-��)���w�eW�J��vo�֝��=�5�[�Y��6��i�����B���qg��6[��+dO��?r1J���̫W�B��[`g9�4]5U
g�7���)k�<j(�[����{�t����ѳ�u��nX���� �˖_�Z]E8�}n��Ҙ�G4�kx/�<��*� �4b:�}4yP0 ��_A�J�E6fB@�)�6p/0����>�� �A�"��p�Ҫ��L�_�m�t(͐z �⤉+7�,�`�o
%���A�0䷓�4t��!EwJF��vϟ�7%#I����x3*@�(���Ca6������!�]�I��OF�r��ן���il)��G<�6��Y�-�i,#�|�Q���5��й�����K7[��J��6c��x�6"LME�4A�;(x
A���M!Qa@�(��#Ǩ����A- T�d֝����D������$��P�;$�`̎��Na|��d�ʂ��yCn�9o�z��+�6���jQ-*���;��(�P(Ħ� ��@lZ���v�{�V�{�R���{c���Z������t���Zۤ}��/�����Mb9���TK+3��˞Z�+�"Sb%�2�Uv^Y;�e��q���q��q#X�1�J0���3��bl�^6=�ԥ��Zi�I377A�l���:�
��4�9�Ad̎�G���A~?�nƌ[Q2�l&켸\�v}j9:״θ��#T����,��l�c���Y�'��v^�e�2��'�@K~P�+� r�|�_L�Mω���ꮓ������P���`S3�ؔF3Y�j�X�J��X����"��]��L�M�@y�m��2�:$�J�q��&[n�|P6p��a�y�P���e�n&�HDEIE#Q�9���t:�˝>Ն4�`S�)���'��2��V���<�������j�w_-��کh�����D���zlb�4eyk��ilk��׸��GX���%IK��f�T-��y�L�Rs=��*�=`�ɯr�(H��B��f�|rJ��h�������y�cK�p��/�?\�)�ɫr�%�z]f �`�� �^x��y�ſh�D���1��J�$����]��<6=3�MkH\Nϳuj�N���͘>�3)ɘF�t�,��ê���b��b��[�_�L����S	/!n
.d����K���@y!)�w�O|vm�ȁteC�v����V��9�"�)�ʉ�®Ѹ36�RFY�q"�,3>�>8]/��^�)o�X� ����J0��>�Bu�3&���I��8Y�%Uf|�	���\l�"�#�������8��7�
;���\_42>�A=�����w��ޝ�m���\r���y��,��6U
WY�d3��΂ �^Nq�Hmm�d
az8$qxF���P�����7�饳<�T���'�}�{'_��nSm�;�w��7뺹����<~�yt#?���]|���F��6�r��m�'�孰!��u�3�W�D�SȞ�	Ɵ�F~�Y?gN�_��c�9���TW�f-nq{����ǩ�q�3��p ���P��u;@��q���̯p�Dcr��8�E�2�s%ˑ���Z��G�6 �j+��8���@�\_N������uu�p:}�ͩ�3������E�la���Z�q���\Q3��O�H"�r��S%� ���`'��Ӯ�0����|�g�0��x�L#Qũ���x�t���T$�1�,
&�PHĦB��fB����)��V�4n�]�X8���ˀ�~��&13F�L���E�r��['�R�����~E� �>�&����j΁:Z��ԇ�>��b �p[A����!%�?�T����$Sʑ�$mp����4��bFL���*�00yS�vI�|XX��߃���V�(K�l�jj)��a�`��؃F�K�����<��N!�Lk�g�����1QȘ�Ɉܓ��?6���z����-	hB#8]����"{�_+kyu:Q�1�io.;C�`/�-���#�Q���YJ��>5��b7� lx��N`��n�<3�P�[��:���GM�o��ldy��Y�JPB�l?�%D��������s�(.�S�HÇ�ޟaZ6���ni�A�L���|4��ez���3��g�B�- U0��2���`M<�s�ƵB$jOr��4�3�2�Y�y݃7MR6n}6��Auf�$�0�%�^���~�G�O.�NC����i�y��}D�3��S,��~�l���y�a�Bč��($��vT�Hl�4�f�� y�2�����Cc�^��nH���R�=h
@ޤ\Bk�,h�4B5�� ����[���(���� tFELt�C�P�ÉS�C:N��si�~tb�>�O5�@��5��|�yH�xM�@��B,ϳ+���}U(�/�` �P ��L uB�=Y�(2*�b��)�����n� m|��]\[!9���'�5.\�m��uO�am�n*����%�Fd�2@���[����+�}��M� hm��.s/�0Vq�΀0׀DO�*G@����R@���� ӂ� x.�pQ
�A�ՠ�5S������f��^g�k;[��.$򩡶\���B�Z+s�0�������T_ϥc���δ��Y+cMT/��v�qt��e�V���t�:�щ��Vq��#�c��u~����k��Ke�z���Y��v[�U`���i�e��zaT�*��)}����|t�F��w�[ܠ����Y��f�x��^7��7$��c�ۡk%��Ȧ��B��jy���5�4rq��+�]J.\�l�1]�լ�J�|}��<%�̎���I6����j����nΦ��$F�׫��kR�񄢿i]������޲��U�Z�e��z#kb����+���-�R��e6w2oİ~�F��7�9V����A��jM����f�E�~t�����B�|�Jp�l�$=�	��ԑpS����� ���YEĒ5M�[���HH"�/Kd]�W��t�
����.�|�!Mg�|{�ɦ�y�l�V��O��V�}4j
��i�UHx2U�gl-�` x��O��ˠP��U�-��OĢ�n�h�����vA�U�ƲDmjm=G�`>kM�tc�wA��8.��ŭ�C�8���K��<l���#a��_�
⥾� ��WA���q�~<���"J����0DϜ�{�.Q�Q�����sլf�7�����s-��6Q��m�7�q/3��<�� �Ԅ�^?��$Z499����7+��y8�V�L��/Sߵ����ڀ~f��.����XS��d��U"���tu�$F��MW�����HX��X��EY:8��lCα��pA�R�
`�M��-h	�8=]m1� ä~֎A�)���0�g�-�<�	���/����z7�5�5aZ���%��%�MFĪ*`�2OLyU��f�cͱ��M��H�߇�HlJ���:�V�,i/�hĈq����A!(��d��6���4PHy]yz�&����M�Fcӕ�bӵ�[*�Ñ��K������@��ށ2Y^���b��*[��ހ�9^�W?(��W�B�|]�æ��Fs/+����X˫�<�Z� �	z(�H�%��c�lA^NW��e�zj�/V�����ѱ=r���I揶������ IIAn?��bPJ2�	��{����r�	T˒]cz��um�LY�M�y|t7$���Ŭ����#���Ũ��a���&��),��լfZ ?��z:�y��16]c$���e������qӽ�a�'����^Y���
��>� J[Ҙ�B�����JB�iJ�7O�a"A
U	Cm>�!�ʇ5i��0*�����`P����3�A�Ú�M��̼�<����s�f�^�<�ޞϦ���9�{L�9�6�1"Fq�PaB7v�d�R�aF	(pn���j��%��CX���O:����{�- d�ȱ��]���]��^��8������e]�Mmb�b͘~������Oo��t!�����	L@$�11I���_�D98q�8܀���'x�TԬP���FI����p�8�ƕ=�+�v5j��5�f������X3��kڼ/k�R׎+��]�.��#�,F��1J�7p�������_�_�������˚;2�6��~ib�|�AΏ�"�N�_��(L(��4�@}3�|a�ڛ�dF�1�ʬ�_��A�V��<��3f����06�Y�;����l9���
��
MP��c�t`���93��:��C����9��l1=��zC�����8C̀cg��»S�@������>��Nc_��^P�a��
��'�:�lNq�5rs$� �*a��dd�7��!�R�@D�$I1��d%�`�GsZ�i���� �nJ�$oͻ���}�	rX�	C��g��i�z*���d��b��c|�B|Kޫ�k�r-�[PN7T�-��+Qp?&���[/�/4�����5��wQ�e���(i��9`��Y� ���k�t��<!HN/:�<Do���b2"�&��
����Vo�(!#%������C���lOy�/�����ʹS9���Ά�[��%2YhN�oC��?	�"8�.�C6B������@Ѯ���J��g��5�WD��n�O�hCc�"�"��z@2n�"��C�azT��{�I�T/no�	[�����z�$�V��.�~$"꠫Lr�hL��U4�X���U���;��=��}� ��Jj,��;���j8[�/h|�@5_~{�uy!�L�`H��d����i��A��N�J��5U;0�~8lv���ި5n�p���[�W�N�
 ��m���y3�����
�3$�	8�`p�@Z��)w6� _Ȕ�9]�4�ܶ�=!��/���V
���E�`��ϣ}��yS5]�؋��\�h(�/�` �Z 
�%M ��F0)n?
ZÓ���Gs�����▾L.;���bL��ٵ|t�]�>̾0�%�����M?��kkf2�F���R#DVGu�I�h�)��i�&c7����&m'��V�
e�?2�<C,����A�3T�Z�akK�������\V�Ya�7e��Ԫ�S�<u��QA_�X\q93������u�7�%SV᫻8�t��s"��̺���]-��]�o���l��-�6�뇠?R.0q��5<,���닄�d�6mt�,gge�6��+OUy�NE�s5_�8�/�������{�Sv}�G%�t�
_r�:l�ZS�LoB����y��(�i9��p�p��@�42��T9��R����HǬ�i��W�|H����~:5*�Dg�o|,�oձC
F 9$��c.~R�o4�+���(��V
�~[�����R�O���k&�=�Ohf/�Zk1�]�>3��3�AK�a<�~77���$پ�F��giCc�E�ʘuK[̺67�o^U�����ukUe�5��D���-�Ჾ��P�>���w1Qu��$���2UG�\�::c�=�Șu�[̺71���}̺��f�v�.	ɔ���W�h��	�*�$�A'�+>�lh3�x=�n��V��)��R��f+i��I��eN̊k��BZ=
3~�����f@s��4̙�PP�O�[\�kUⷺ�G�NIs�v��#�F�P3vI�=�7_Z]f�<�f ��M��(P�<y�����Z�2_�<q��K�3|V��*U�|�Y�d���$J�|=���B����E,4�.\�4@$�+e�����w���N填q��1���Ac
N���t��ۏ��jRZ�����2���"��
"��uJdh�L��!R.x��7�!���fDj,�r�Ƭ�uC".��|�ڮo��_\$����SLq�k���c�N���ho;�}$��hj�Mf�ɌY�\�Ŭ�k{%d0r�-���/!�d(����@�z���'��N�
5�����
�#:��!f'M����#����D
ءy;mWO%����:�T�<[cqN��n��}$wj�Gt����s���4��ۢ!���U��V��*����pua�e�}����a���ݷ:�N8��>�y�z���s��Џ�t���`���=�XAqt"f�|��DbY�Hc%ǘb�QECM�a�P��!���o�z�~���w�^83/WH�"V!>��); `ѴV���	���X-fll� 3�&�	�L��ں�N=�l,����01���Q�^�ڶ���vm�&S�p&J-kq�[��R�YWm�iRT0I^�@gݎ�D��[||�;�|���x���4z?�ff@�k�^!:�<��$�A4���2&g/�	�Ox3��y3ϑ��V��qD��_<��Ƅ�ͤ	b��}pu�"][]clҔ��B��̫�P�_�wj��A�^`�W�R]�~P���!���p�gڞ�TʦK�+����!�^ږ��̵I�d/���ҙ��^�Ƭ�.IŬ+u|�F;}�b�$k�F?�%܎)�4�H�I��z�6�/�7l��8�d`U���)YYW.r8yuuuH$�N�s#W3}�0���\y�1N`����I�ܰ!#�z�YG��#c��# , \�<fq�p�	�P~��a�X+�P��+q 	)9��!�kA�N��
ؒ*V�4/�1gA3VF�+o��@����!ꅃA���P�_�{ 
0N�P�`B�+P�I�[R���N˺8}���)f]'?R��ɪ�I���\��1��sW:��"�D4��h.�
���S)�7e�������oq��,���"�����&X�0� �(H�H1���Ť� n���CB1o�dY�e�X�REف�_qH|�v����ʿ��C2:�i7���μ���L���7Lȯ����x��%���i}��*]c�]�@'9�Cy�SNz�шr��{�c�!��@Ly1_5ΈɲƏ����ӋY+A�(z�~�/��)�MN��)�̋��R�Yוb�U"!����%h1m����}���P�$S$�k|<��0��4�­�&�H�H�3N���wj�����b�Er���p%��iA�s�X +�0�Πi�K5�̘���Wf�4����_�845�D����&���܎Ɂ&#��vPS�9�|��)�]��t��I/3}�������E1��|�*�%OWK�l�A��?�̠�x 䆖28@��BL��0=����Eߨ_���?u��x�a���t�)�8����b�Ħ`S�{ND�K�Ҟ��=�'�N�I.����Sr�q+�	O*�ה�c�X��-��8-�[ng�t �FBy*.��<��}�$##p�hі��kaJ�#;�模��J*"""""
���`�!��5B�������T.!�%�1��W�Q������*,$��b��x��y>�G�!`�<�H������ �_-��e	��I܂�0�;�d~9��G6�����%��K��b2S��ڧ�B����� T��'\�a��Y����W��U<ޠ(�\�%��� �@8����xB�͏���'�r�_{&�.P��?�0�]> CrL>�$�s��T.�`Q'~�H�������g(fGu��X��śF�K���dXr��w�����
���%i4[I��&��}f�g�	�%!�+�US��O8�*��� ���Z��^��P0vx���hv˫�L�AG��e)����h�Ѯ#�IgW|-�I�N�6����ƛ�_h�F.%}� �qz�ќ�Q��t�I%�MƂ���m����y,z�X����;����5��K�Vi�t�\;SU�����q�����t ��%>#D��g^��)Gm�����i��H�S<_� �
(�/�` %X ��#K V#u����p����Z��b �����A}��|5��3� ڪ�jFF3�T{[��φ��"�vｷL)E�z�s(\�|N�"�ew���p�1׽��9L(���<�y�t��aa�|�O��2��4�[B�F<,,��+N�8�r��)/��r�� 81�UA���J�]j��!��������"7��nKM87c����F�Q*�ݵ�Cgr��p�(_����r���Z��!���0Uy���ťۚ�HSO7ߣTΝ�ڞ����:�3�48�֩{a�f�+d֨T�#��͝��_���C(�ݖ�'���Me��u�TgT#mjљ���<�S'���#F.��ڑV2�� �K0��x�m	�(�h��(}W��)�c ���t�1����������C�Oec��QJ���k�!���>���?��{8�����>^^xJp e�SQ�Q'6�3�`5�UV��x��UY�\���ijR���Xw`��8�o�G)Y���IJ�]ۜ�k��lk$;\����jI���<ʬ2��1�>�v4Q�M�_<����`!s�U&*}(i(W(�i�1�%�RI���Q�(��%�HS�/�}�������g��|b�?<[�X�|�R��B��%�	�ᐛ���D���KM�|m�9��#�#�k��Kb��6�T�R~�~�$ק��c7�D���uJT
�5����C����	�Mp�C�ك����cj��߆�J=D�E��
>��9a�4|����͝"6���>�>�v�Ki���
:фC�W䥾��=7�:�3�n�欓��<�{3�w�M��E��d���}�M|81�6��˰���[>��8���� ��q�����O�q����D����L��s��4�<z��|���֨��m�}�#��E��oD��gmEj�"}���>W���8�v?�6EI+��78����i�~���ϖlɲ�M��.s[$�Q��VuT5ݵ5e����N��G��_]i]]���H��a��$jb��2�L�H��%N�}PN<P�M`���Z�H}�{;��E"�X���V*����ޘ�g�Y��9��i�Qʦs�V�δ'Y���b��*��������`\�SwR6�=��)C��%�J��*�4��f��|p'g�6�c�0�,�!�c�":���䇏$���,3�/�fo���x���V�tv���Q�������E��3/�3���mt�����3D���X���o�I�.Xh� �b��µh���eW�W�]�<�� Vs�伋��т�J�`16DA���,�aV��qE_��᠂4ᰇm�큿'7��s��͹���9%��'��.�7��,je*
+��=�,�0��Q�����D��<2��hQ�����\�s�&,�
���a�V�(c;"�'YIR+�`�Ƨ��נTG*��2	�&��@K����6�!TVp��B"�C�)�=�M�ݣ�'���ۍ�#
�nh�H��N�/9�9�f�~��S�=^L�b��ɋ���QW�[ok��w�+����8�|���ʴ�]���jùk�����3���_B>�݅13@����="<��Ф.8�`����\׌VU�'�:U�İ�*�\��x��!;�q�	1�.�A���Ԡb��!�(��|��	�������l7� Q���|��]�ij�D��k[c��"�&q�C������;�C�-�:�v��������DnK���R�x-�#_�A� =�����X5��tIg�I#���\�b��-��#���- �I��Sv����>���[��<��Z�������"e�"1�Z���a���<�$c�1�<��K�2߼�
/"D��0��a�����1���c�b|,㱰�;`�v�.����ʻ�K�t��d�>J�l��3Yj���̖��6��b���f��ά8�k�O:�{�cǒ�X!���
�Y1L�2>R^.�7�!��(P�W��t�1d��8U��v�s�����ap��X�ꤪ�Nδ�k��Xf�����K���V�ݵ�5��$�u;V�ny�/��vLEa8���Q�p�T�6` ���
���a@ZD�Q��A�	����i��*���O**HTٜ�˯���t׶DݵEm2wme1�IS���T{��
���6�&���`C��AɆ��D=Af�m*T�gm�����gM	  {��Z��d��Ia3e�	$�)lx����x�w�&�qLnNg�ֽF��d�ޣ�,�﨑�DdFFDd$I�`��������Q��;�S����Ȓ��ۣ�2��Qõ0�䎢�F�v6y0 `�3��zE�+��֙  y�t��JE��G���zʦ���zgv��c]�M���O�RLA����S��Tt���9%�ō��$�`��~6� �<4uqHN����s�ϐ��;�� �����<l��	R����b��ٳ�lˀ����F�
�����X	 n��n���Y�H>=�g6Q��E��`0���ꏏ���n$�g�w�9�u�y��#�o���jş}de��n�'��4`���� ����1�Yӥ��<a�xY_��(f��O~���MhH�kK��]8�������\�mӨ��@�6�@\�4
`�r��	ㄆ����90r��7=أ���%�eRL����L�&&���&u�x�~�-�h��~��������rG��«8����C'�%�Kz��O"�E{����M
2��]M7:���affC�ݔ쓷uZ&(�/�` mX ���#Q0�*�Z�L�?���a�ȏ����,/�!���Ԩ����  W4�`s1�H
�&R���^��k�Y�.1&/���!�Z���&IR
8$5�rH١@�� �H��g��HZ*�)�C�Td�ՑPTw4��\��	uFݐh4.݄�x����N�f����X=�
�5fZu0�c�{y���\�"�i%Q؊]�y��E&uxv6��m��2ӊ"�}�l$X,�	��}����ң�+ G�)�|9�A���	4�d-�>�%�/���hϗ�����ZݐxBhi��'z>�m���W��^5�^�5�����`�Uk���sg1�VQ�x��\�oNRdR���M��H�K3�=�/���c���&�Ζ��T��EjJ��GS��Z��|IK�.7$u/�^5k۫��n���]�e��s�	F��|�ԙQ;����-����%'�gfF�s���'�����,|e��8�l@:�o�v�bqH�=ʮ/2	�^���V�F�?����cV{yn���<W���s�Ihű(l��\�HgԆ���T:Ai�r���R��c���M�
߮�b��k�;�0�tӪ�8�1��}���i{���|�ױk�5��*3�cf�<���H��\��1õGy�{����v�y�u�X,a�^��^�i���sW��C�((2���a�`�_�l9���^_p$HP�g��*�ח_�u$	V�A|6.{��x5pJZ|�<��9��!��m���ͼ�<�v;��@��9/��2��'/=�" ݡD#��ℑ��>2`?g 8��f��*�K�-[��E�V�K�<o�5r����s�����l-)�#.=u�T%�F]�h����W5M�*M�{]դ�;��sif��	�$߹��D&���XdxpoT("'K����Y�� �2�s�´b'ĉ��/C
Lؠ�ʸ�ȔD��H,�֢q�}�긦cZ,�j�|y�ɚxy�	���\��\�c�B�Qۇ�{-G^Ҝo�ݦ���-4R�I6\��ȳc���P� ��e���&�r����̵����k��a6K�:���|IImqu�Sz��^;�ܑ��@��q����d���Ғ�����żRT�	yj�C���%�K+Q$��2L���P0�{"C��0���҂������E�iM��ɒ.Ƚ�,"x�˯�R�*�2��`19�dI�}6pw0�'uNx�8]q�T��
��� �����%�����M�1m��)�ڍi�Y�f���
�y�/q�"�/]
�7 ��s⨒1��j��KI����_�?m�{j�y���]���3�4�����T���t�6F�h$��Pv�]T�h��l���>s@�Q��<������t��7�h>s���9ƓI��~����T�悳� ����{х�����7�*��mi���p�?z�}0w��Z�2���زE'��E_��e8.w�����=��К�'lU�G�-���8�Vo�`����0w�I�cJ��"$L4��Ѵ�(HA]���)����aS�X�*��Ԡz����T�?t���2� �f4��b���F�{�v�U�s�N#��+7�S�o"�S^�S���:��%P��eT�AI���]?�3o2�>j�I�� 6vi�̌�J73\��/�-�(�����_r�i	��֪r<�F#�]�CbRχ��5Ea(�_g�p,�P
�k�����!�>J'����=�}_�:~�(	 ͣO�+���rv�k�iX�����S��qc),�ʖ���q���V��t�j�U�����<�4O��4���l(��}��e�w�
UQa֩�r�bj�� ��1 ��WRVeV��r�����=��׻�m��1w��Fog�w�}��M�E�P���'�d�O���I�+f �R�0e�h�&�U�6�¥G�R�h��0�4��Qme�N�{r]�
����F�<rd�0�z�̑�c�J&�k;ʇ�~�D���%�d����ݍ�e�x�.ݾ���]���Z��L(^�,X��V�@2S!D�ab�"��3I�*WW8l��c�x�s]O����3��
�y�eH�rɦ�{��<��I�P��A�����������8�jV��ܹY�Y���*��mi\������'O={�|��L^:P8k���D� X���9��! E� Q�}<���� )�~8��+�=�-eS��� �saG0�7k��:O�F2���'P��%X�"�w*(r@�"��Qe��������P�}���9���/���حP��II�X;z4	���ѠC�!�	DD$I�`��1Rp�r�l��!C=���W��a����x���m3��=������y��7v�"n�!�-0�/3m{��|}X�ޖ#k���G���2<ˠ�������o6u�0�k��=c>�I)g���GD�����E���wX�s�J��ᥩta���Ɂ�Jݶ0�XS�#�jH<�����khR�c�9:�J:#,�8�$-T�JQ�4q������k$7$���9�;�Dᶁ�DQ���y�)GȋC��D��71�Z]c�Vg����8l��{e�V���GE�>E��(Q&�����v�����M���H�U����S��Rj�;����f�\zR|n��	m�@n�=\g6��n�������n�Q?�rw�ug]�/��*��U�"6�EYe��9��7��4�����z�2�Z\�� �,�=c�a�Re�}�y���%�"�ŉ�[��0lHҔ�O~�3���ѨV[�$�b�g`�ߐ���u�mUq-b�l��v2b��(�/�` �V ��@#L V#uR��*�X��g�p�@�c����v��[�ܩJ9��#9��1��!.�֌���0«|�w	y�x�K�R�;�J3u��OD&��A�h�Ϲ��ΎK��;�G�Cg���=c�i�X/�h��t^
0*�͞�_=��P=��Q�G�����Gv��w��UԳu�Z�R���XL�kl�eΎ��<(6d��"3�HR�Э�[�k����S���:��4�Ŵ[?��Ywlb��ں��qW���R��qv�6��LFv�r��l��^�e������2W��>�t_��c��X�cΎ��N�; BQx� �D!AD�n��������Z�ٜ�ͷ9��q�Iʧ��G�_�#h�q�[�����B�5OY����Z~�ՀA���r���l��ْ&I�uֽ���zY-S�����
i��uF�t%�-�ϒ3v�V�r�6���t�x��W�ǾJ@;��`�סh�>�}J��Z�7�f�:]��1�ΎyFwӯ��0r޳�gd�TK��zX��>* ��wMf�
��t�XUKY�kf������qFv����'����|�����|�����(��|b=���J B!�TK.��|H�A鬻۲�µ�X�]��B��
�ҋ(o%pn:�Q��d���s���G������g�}
o��d	�"U����px%��>����iGt����<|K3�X�ˡ�G�����H��r1.�}?�|�$�l�j%��Z���NW$Ӵ�t��X�Ne�n�'G�*k�L����ɠ�f�B;�M����>	�DPRl#�����#�l��b#{7of�k�����$\��\�H,x]��ɮy�j���ՀA�����y���9�	��zDK�L�$R�J9��9$%��JL:]�vP�ֈ�܆qPvo��@|�$H���x��S�� �v�}�X���#����g����*p�jyC�R-��J���d�TˠPHK�$�e�H���M����ZN��&&�6˗�ь�@'3�S����53�+����
#�mJHfl�F&Q	l�>���0�"�24a�
���h�ٳ��/_���X(,y;���7�|58�Հ��nTOY��'�Q�%�X�S�&m,�&��u�g�zdҎ��Y����f�h:xF�+l�c��S,ʂC$�˲�x��^ǖ0{��\9����'�=N�O�隤�u�⒤�A\�_�[F��(��Oc�k�9Ѱ�d
x y@g��^�l7쨨�Ø
�t���Ș$(�@P����9�:�x<!Oȓ�nh�ϗ��4i��;iҟZ�v��pv�g�=��D��1��k�
kT�@��+,:u|?h{���G)�v�I�����C���AK��eS
}���CM�w�a�}2�2��]�I�B����N�J�vo:�Q1��EW��֣4���N���o���;? ��+LK�YG�
s�TD�-Zs�(�z��5A���@ ���)N#r�X0AN(p��A{PI�%z�~B��(�d�?wb8���xC!0t\γy����'ڤz�j�T�3%M�0�5�$n2xa�L<�W�.f���n��\��>�0AW���g��.�c�*���Z�j�2o���d:]e&��[>s��Z����eH*����%?�l*;T`���=��R��%0�r��<�2A~@��%T_�^;�b�.Y�,)��r��`.��(�Q�L2��veb�l�'�ʅp��7+��s���p��+b	�%�=��+�O�x�aSz���;���0r����<K��T�I/�T�����,Y:�e��M��W�*�[~�OFaQ���@Q��m�^�kǈ�ȕ5��DV��2�R=b�%�,Q�%&���#iCϣֈ�N�w�"���ゝ�u�0S�h�Pɧ9�L�aÕ�tS-(2D��IKl�qA!��AM��8P��e��M2O����rR������� ����2��0ϧ�Mצ�j�FE�:]�(��Q1pj���?_��;���Q��"��D��*�Z,G�j�2��|���`�����	đ)<�X��'��v�W1��~7��7ߣ�t�ֺ��hM�nę�#���H��!f���Ty�a#�r����|��r�s�_�g���Ȍr$��R.a����B���a���8���C}�wm=��M��	�2e'������_a��}m�Է�~䩎6��jd����K�h5Mb%�uQP�ж�XcI�bn�������㨱 �5#"��HI���"2�`)SUm��uX4d�!�F�#;}�����<���q+��_E�Y�74s%ǀ�E+��[)���#�K�4h��`.����4�Џ�OK9�������1W�֊�S;9 ��勀Ռ���qi
\�Dh��[�͑�R���I��B��*�~H�.�V��->�K{�����4���
�=��nL ��C�y/��)�}�4�#?�t�>G�t|p$2�����Pylp�(������W��A[���[b��I3gZǦ���N"�Q�4an��H,���뽪�߆Q�q\�E�Ŵ����-B��@�,!mʵ��HIU�3�z=����U&�N�R$2�ã�p�c� �	nGf2 r.( W��@�U��\�<j�'�	V��Z�Mc.S�F��
�����	�ٞE�Ω�d0l�f|���%P�;�=��EX}%�9�^����C�'v��t�Ȅ��!�B���H8�� (�/�` �Q :� N Xu�0��W�����@�{��F�˸��2A!?�l�����!����7K�ޒ�tŌ�<�QH{�B��Iۀ���n�eJ����>u��0k_G{/�p%�? .��y9\�Y4`pH⍌����w)݆�8bPn�*����G�ZμTs��$Q����<`P(,���&�Ѹ����V�1��62�����Ğ����՗�����O����k����o7��D�ŷy7>'�R��/�1N�W���22%�RK��$������a��G�MW�fӵw:���ε>�X�����mi(b�mt�t�l��z�A�(��k��/�,բ�	��Z�	U���dO���MW��M׳)��r�V�F�	#K�$����%&�0����`��b��,O�$,�:"eeE����γx,ژ����k��p�hT"�\%�Rm`�w<��'!}��{�2�Y
�����ٳ'C����t�A�����ړ�����\>{rS �i�tP����ɒ��&a�H���!��H4�����H!��E������썒[d	�nӔΫ�	O�y8H�ѸT<
����G�ڮ_MX�@T�����4e��S�5�H�j1ȴ�
e\Q+Ğ�|��c/s���t;�.�tl���3<�i�Zػv�[5��Ή�O������}o���UU��� ��M<��x'>�t4	}��ڒ4���قBGU�m�#[O�h4�t2ʞ�|����p�B����t��xl��~�о>�\��Œ�mEJ�� D�탐��=>�	YL���=>��<�;s\#\!�AF�3C|��A4�@�f&�ρD�=�&�Tt�Ţ��X��<		{���O������ձ�P�	����?�|�bk�T�P�1�`�䓾��ȃ~�K� Yų��2��4�W]�0{'�5����`�Z3��αl�h!�[H ��J�3��k��ڮ'!5���q-�ӚBl�
�l��A�l'��F�Ai#����Ë������.���"�j�Ҝ�@/���{���p��<�J��NTE�6BU�D�P�@76]o96]s&��&��X��z*PZ<�zL�<��R�;@U��;[g��;6]w= ����u6w0�U��$Y5�M�P�D{�+Op��Q7�҉V	Up�J��d�P Ͼ^-��q�]EU����cO�Þ�W�[��'�Օ7ᢺN�?\D��y���˦���Ӈ��� �.��]�����!�ܡ���q���O�Gva9��
��(�diY��_�TJ@j8���c��7�Zia� �#���:���T�j��*��i��{v>y����|� p����u6�t��V��(cf�,��8\^�e/��y�X�2�'Ǘq�
XJ�r�zёct�����߈G�`& ���jj���F��b�Ej��Y���7��V��Z��4L���\ta,Ȟ���*>���|�!r��X�O�M�|B;ڧ���K���M�*j+*�5[m��ED#�=+��q����:�f̲�И�"��ܰ�-i�%���Ч�����(>̖%�b�\�Xɇ4"��X5'J��Mןnu��cI�44o�s6]=�▁>;�@�{Ȕ�ͦ4"�'�l��>V�M9�6���j��;�M�����*Ȓ$@@�b"v��J1X�LI�T��Ɂ)W�(!! �z^��eɓ!OCA�^eD��Si$�jQ(�ʫy{��0Жʇɖ��ø���հ	�:�݆�y�!c�Vu�6cn�o���YZ�HnO)����2>�� ���o��'��h��7��� �$��zUѠ'!5�гu�٧N{ę��4i��֮�y}X�l���c}e�9U��'���Q��~'����SC������kI�"L"��rF"z<�ƞ�W�}��3�˵Dz�>��6f����ku��)�#�y���+�WeiY�AA�)� �P�5�](H���ݹ��a�u��i�4�q�M4���o#q.��V瘓M�w������'�$�cyYf��vr��[g�gr�'�m���2�J��,��� �� S�IA�p1�P-b
IIA2#tJ����Yx(�(eM���z~Q"m�<]eG�;�w����N�D+��F��&X�W��L�oC�h�n��p��>^~%�$Z=194��~�J<�Uw`kw�b#�E�y�+W2����޴ï� Pʜ�$&?���p>o���,�b��iy��K7O`#�J?��;Y��$���Ɍ�uF]LN�e\�)
6B�(=JY��n����܊kn��s�

�W���+��*��Xo. �	`��E
 ��ſ�'�a�b��j��I���v&aB��O��DS�f���&�&3z�nrc_~s�S��!�Ie'�5ɀG1=�gZK*,|�[I,������lv8���\�KyK���Z&�t�ųL.2f�[����������c�#�@�֡	Xp��T8I��/��L�(�1*<I�
���`�Z��/��B�l�oi놈�%��x~[���Ut}��y�P�|�Fi��2�U��*!,Z�rt]�*0��<�=���Ӱ����Z�c��@��&m�n�$��� \(�/�` uK JX7 lmz�IW�촠�l�k�XÒRI���d�>̡�ٚm�7����-e
���Kג�X�A<��n%�y�k��ME#�X��L�*��z�o|b(���i� jQ��N� �n�x�$˕�F3uQ=KF���I��p�­٘XQm�V�R9��5�9�i��5O~cޯ� 0�-|�<���MLhތ`��`��@v��;���hp��ϖ���z��P���7^/���l�c�{�VJ�t����SGBSϞ#�]���j8n�t;v�:�.�u��4j����겆MC�aj�z��&�웺ܻ5���X���D;�p{<]�J԰�19LS�cĢ,��Q��6�-����b��> �>���o����籐���P8�C��gx/�]_�����0��[���Sh��6;����u'�� �u�����)h�6�a�L"юC3���jXk�ٛ�7<�@�랼l��󛃟*���G�}pr,�-Ӱ.�0Pf{�Ng-����_�����U��?�����M�X1-lE��~T��9�@
��i��VW�x:�9[���bF���bfٽ�Zj0<2i�gZ�4s:�-�5�DK#"�E�=F,����3�̢�͒�b.d�j��f ga�2x��d�����Zw'����V/�z��F}:W��R$ϟT{� ��I��z����%��6�a��c��ܯ�N�>)W�@ђ��^~$���bY�'�A��p�ǜ�4�����|�7~����s���RMhݝ��ot��i�j	]��탂�����2���Q�&�q�fev�\r�+�}��~A�n��q��k�6�뷾�<ݡ�'��1�@袚��}�1z����G��(�g�薚�uZk:<��L�-��+DŎ[2��Z2�}�����Z��]C=�x���������%���;9�Tu�����c�>��x�l���Y���,s�Yv�����y�+Q;���ԍ��	��Φ5��J��z��3�г�ԉ4��Kؙ�VH1d��N"��v��%gW_�|�rw��,8ýK�����W�<�ۙi3;��v���,��7��������om���gӐ1��#W��P��+_�h��1e�K(P�6%�aBE�%@�H�hf���v��᪺�!��ga2��se�Y\r=dg�=tB�ñ�q�sp{�i���<�pv�Z�n�����^�V@�����$��)U�xqB���0a��aH0��>S�Q{�t[��z?y�0f���"�0,q���e�S�j��t�꺫%�]���D�u!WU]U���D&�@����w�B ���^s�����`��`�5������ꆊ�����?"�n�6ݣT��S5�6�D�z�6�m�3%5����*�Ģno�U���]��x��E>~��W��':P�7�:��N6�R?�O���2�K��h��=u�x�nNf��/�0�U�گ��Е�.F�ͥ_�_(�°�N� �^1�y����R�:�Q���-Ȯ��]hf�~���c�Wﳨ�gY����%i!���z�:U��sw;]�cg�7��1{jV\��f�E���~B�fSN��JG����#:L���\���wtmU��ʜc�/�,���F��(M%���e�S�D}�k��k���+��o��=�h��i`Ay�B�9��c>.ș�s�����^�L�!6���u��N��R���b�7l�l�Q�����+<"�`�J,��i!��׀-�%�86Y�Ld�9�M�Fa�+&�A�dsO�ٞP~c�{�H��7��κ;�?����э�6z���ro�5̀u�L-4�B�鐊0��84��!Sʙ��$I:�b����
D�1(��iڿ]�5WP���/z���_w t�;��1\���	o'�Z�m�,��h�����j����B��������̇�3i���W[��7g��I>^�*A���|*��c�(	C�� ��F���k�}���oП�Z|:Hr�/�E���	:?��{zY�0k������l���~��=i�U�vD
�-�o�)<��]�P�EҸ_%�B֗���٦� ����W`U�P\�m�r�\:��x&;�� �ﲅD�
5�N�EH�#	2�Hz�j(�d\/��]��DX)2���}�x���1���w12�3��2|b眩Mj@%���#�2�l��1��\�Cu�{QW��2$����%&���M����[ *Ԣ��UciD�$����uSA��u���������9�Vھw��l%uw�XWS�y��@'kJ!���:Y1���tp�颢 ,Ӡ������V[W��M��CY���3���Fez�����B�b�	`��xUF�18�N��X�`%��/!>�\ͳ3�`�'��ؗ�����*����5�(�/�` �J �{�?@�q�S�]j�P��'�C�#�pخ�?�d�������i���f���T�������6��g޶7�2���x:�r���dO�~�$V��r��}a���%��N?�n��ח���{n�ɫ6�l��̴��?3}��Ro!� %� ���'��>3ͤ��1��M`�ZM�&����>���v wc <��<��1���e�4FBKo�:m@��.��_�1�U {�O�%��)���A�W�y�������A���³�L��X-șV���1�>u�z���/���^�E��n>��s�n�r7N��n�qO~��̔Mڦ�����wn��֪'/�"U�^�X�J���:[�R���),���]b,�-&������R���	��5�:�3���h�z�i�&�H8�n)�� �rIdyH8�%�j#a�}�+$�[6�\j�/[T.��p��6�J%�����cs��Ẁ�����T�n\��܍WY���c\?!�����7����S;WNPA�E�74Y	j���TraB�Po3��ӡ�VE��R�����L
_^��f��@��w��n�YyM��tF�C:����.]��hZ�r2��Ϝ��)����N�m��31����R���Q��P�󤭬|�j_@����\К�k*?5�^��5�[g�gk�X�=V����I��X����jg%_���J�rO"�?����FF2e���W���!D
t
�uߋ����#EQ�U���ccR'�ʤ�oֻ��\�7+��U���\jia�C�(�E�����Z3�Sr�����um�Av����gU��eO�^i$T�	�5�s��G���NHw!'3u�'�^��c�H=ܢ�,_{�����������q7��mP�ʋ:��7>pU��<�����>]N�B��1S#�3S1dala;C�87�0�����o�'������:LG�R��)�Y�3���'�k�K�/������Zs���5�}��n��܍���򪧤�L��>3=�Wq���3FE�vs��ܿP<_��Y�����ؒh��W��bL��Bu��	��--��+��nV�ʓ�? _�r�����܍�Ey��f_��g���?��2l�M,��4*�5��Nje�0������4PX�=�oKw9iJ�Q6z�;�W���w�&�"Q�|�,����	���mf��6����X�6c�GIk4���l��r�#��@��K�܍�UT�Y�O�WJ��=��l1�%}֝#Ag�>8��N�H".i�+o���~����Jh25z���Y�,��\����ڲB������Wn��ח��t��o��禮j3�q��܍�q���G��W����׶�ܿ�ߏ��w�n|��nL#��ʿ����=mȻ%NZ�����p��hM J�(�'G��=�����^�?�t��v2r��Y�CO�PO�z��?݈������}+�U�*�|�ZKWn[����;9~+�]�Ʈwc+��n 5�x���4���>��bg���>@�a�� ,�6ԙ)�#B���5R_��v>�0����-����D�d�6'y����k+嶳�G=�GUB����Hv�A5�D6"C7�D���ٙ����LS'UvH��a�"5G�M�;�N��Ry�ʶ����&�}fڥ�=S�uoh�<PgU�j4�L=\'�\���@� ��@�u8F9r�5�ԅ<�e_ve_6둰z������r7n�TS�l�^�	yqO�����S��Mh�[Gӑ�E�t�)fx3mk��S�~��%o}�C���
��(�3���0�%����$I��B����4��)�$I�0��ٜ}��LO���/���)�I9���ž�T`a2Q�͈ 1�]9ͤ�)�5J��:Hc�O��j a�I�n,K_t���q�$���	��rF���y;lǉ��6�\Q=����7v��
��g���́��	]������P���dB�yA��]IX�'.�0.ǻ�����9�a\h2#���`�C�J�hW07QԎӸYm�e�L@ )�
hPm�2��
�x���
�!�I�m�"�A�Yoܥo�=�N������A]���-���$�û7L��I���N�J="�vC&��ɫ�H0t��&��-�]}3�T�N��V7�,�L�(��d�eQGHHې����%8�NY%�,��)��v��y��W��$�B��ƹ����J��/M[�1�l�SZCx�`��S�i�Χҗg0�7�����$%Y��3% A2��ъ�6�H�5�"Y�f��ູ�.o�ыb�=0.�6�H+���h	P�/{�
�˼���&����>��ȏ`�	��~,Y���E�� cĚLV�X喜��$�2hO	\�{���pU�D0@1�(�/�` UR ��!O �*�ƪ+�Y�<Sh����k�?�5�.m@���ƥ#E}��`�w���`���y�_������6���z��V�W�.�2����*���k���v�+$ϵct��2��W�g4,Ւ��V_�W�eE����Y�uB�O�s9]�֦,M�kSV��t"r�xr�"sWBB�i<��	Fs��`�Ol~��ɳ�W�-羧~M�_��~i@����[v��r���iT@���֗ɣcPwp���+���Hϵ��+B�)�2�����T�SB�Hg�~g�6��u͞#>s���֦lmlSv��ڔ��8�8ױ#��,,5D�<N��版H����4�?��l1��>_���=��Ns����N:v'�`�;:F�0��Ӷ��m�c!�<�� {��k��O	�}G�-��{~�8j�6eg�6ee5ܦ,��C�Ӧl��Q�k��@Ns{����;�l6x��r��9b� y �;P\��O�A0��Ll}a�c
�v���x]�cLgc;Ep����«�Z�>��*��LC`UO�ܭ���C�A�a�Jo�mc�����A=�6�_�kTG3��6���V����&' ��NE�`�3Pj�A��� �F0�l1j6'X1��*o�Ց�haX�a�\��U>������l�&�A��`�aP��K��0�4\��zd�<e�:�'���9�P���[��[C�5~I4\"��lZ�O�?��4�l.Q�K�ͨ%b��m�">�%b^������'M�2V���%ii�\�6��gx��rl�
���\���F&;�;P,��qm�1�1�Cf���l+�~��f�h��_ʠ�/�G=�LS�9]w���U'r�Q��c��`S�*����)�`�g�1u~ʞŬl�6�>
��gZ*՟��f��M�+����>ֲ>N��ڔ�y���#۔%w:ό֙��w^�:FE�;
��ec�y\e~�$���`���V���9�޸�6�����a�fS��6;�0`+�p(T�^O���P�sc.�}}jmF3Z�)[�ڔͱ����s�P� ��S׹�8s��z�%`��eS9�L1��e;!EO,k�qR��y/<D�{��8^h������ܦ48�&w+��8����d5ٻ��[Y�w���A�zb�4���&P���8��㠟��-�^b����3�Y`�%A�P���&A��gS��J��`�%�}�[e2�,�p?%�6�:G�&�i�F��msoֹVDTs����n�F�8_9�f�!���qQ�b<�rxz'P3���l'�`hU����0Цg\��P̒�ԧ��[�b�ӵW �z=���p(��m�at[���n��?%]�k��l���I��S)��i.Nc�5-�G�D�>I��ش��m�M�Z��:�d�\)N�9�n&����0�	�`ZR��e2H\���hС����%���`(�è�v�\��N��̀�d9��J�!��#��{��A��2�DIU��>����z.'%ٴ�$�&�|��d��f��ݵD�R��ZD�B�9�t�jW7�g����MY�y����<���^<�y�� ��1�O�^�F�X��<�R%;��_Y���;�x�(o>�UV��cUYE��(V^̾��f��
v��t���u�Ɛ/0����j�BJ(Sv�S>��|��(!?�$������.6VVRt��?��#e��s�\��[q�n=qkoI-Q�<e�3�K�з[�,O>��I�:��C�aS���ivԲH�E��P�C�o�ߕF��pm��ԙv3���Y	���/��8���HLj���]��6�ʂP�Ɉ��S%RAPl+l^Q�4*I:����"X�2�S	P��32�y�e�/Aa�
�/��<���ԯ�P�af֯Lg�	kS�����q��8�(�K�׎���CS{�"�]@���R��$���y��?�Չ���|���#K��(�Rb}e栟Şcf�,`L8�Aa5�R&\O_o�W:z�L������Iʰi�I��4�~G�MY�����}n�O�Y}q�N�O	���v#�$��O�v���P����8�
R��9��2��@m���v���F�ѽԱO�騑(��D""""""M�6��� u!KSR"�$�_c���Q\��#��xv;��=�OrH
����+�[�BX�3�	,8s��nE���Fު�a���\T��q�>D�,hr."Ѥ��뮱���Bq6 uk*v�nL1�D[��J��j���ǉ���p����3Ʊي���c�`0��]YK� �)� ����_>��}��R�3݈���
?;�Y���@uaf����<�B,�3��$`8q��:ʨ�pnM<p�Q�G�u�����Y:9�,�zq���'���.f �T��0`��=N��-��4�eLt��y �>�xg$����DcN�SkZ��6 �Gp�n�k��+��[!]�c̋�CIsX	D18�Sw?9A��<U˯�J�ړ%hX���G|�1������=n{_o��s��,u�G�B<8"N���&��1��7�iEz0��Y����Ϛu85o6���2�Uh�'=�:��R�៥�0Z��xMQn�tj
���O��<?(�/�` -U *��"N �����K�S��Q���EC�a@�5za�X�"��q��A�k'4� PAz@�p�a^��Z��#�hm+�C���{��=����%�[�%��2g	��C��%<"�>x-�t�M�gvyC$8v	C�^|��x{1OB8i�C�C�PG�Szk(��rfY��gk�7-�Иi�����z���Յ��xu�4^�4M=^]�����?-���1���T�{ya���K�����9���g�i���A�Ju@&F�$�Ցl��\\x��b�L�UY��%��DYvO���b����j�u��\��T�Cg|��?��M���SH��O��?CB��
��,s��W�y��43Op�C! ��b��;�R�Y���-��dMI�=N�t�z"�8A��/|��g�F'�D���?D*X�x��Pd(�H�r��~9��D��#>��%8�����4�e���o�m�R���6���\��XVzk���7�]���ܭ-u��LU[M�y;J�P�աe���b��e�>�4��E�2�va-��}Ҵ|3��g���bM�-�����Y�'��Ҳ�s���bA��M=��xug��k�y�;}�4�������C�e� ��� p�D���`1�6��:[J�,5�6�� ��#p��h��H�'�r ��:�&��,)i�ʹ��K�W[����{�.G|�в�X�T'��q�1�9D˶�	��S�Pz��SOC7�K��!څ���M��K��]�D�3$���q��\T��AD��NB������O�Dd�;,��!�8}"̦�|aRˤv_{�I�*;�D��@���M3hV�m�Z@#3���@	���*���I	n��4o�,>��;jy��5�ljWH��g��P�6��>籪�.d����Y��X�N}�N�gn�L��� ��x���=�̕2V_�J~�&c�̚gb���3h�/����>���34���?�LR|I�֊8�|�N���4"Aݯ�l �d�(j<���c$�.ۆ8/�����+|�#{$n�ڤ���j֔^����a)�!�o���8�]���k���f�xi�����L�x�ۑ���T���)W���듿B�[�g_[-E����z��2ޣY���>o#>�H�2}���@�����i��DC���!�uܼ�(�_;�1Y<�$�-�ͧ3om��CgJ��E�i���Բ�5O�,-:j�~Y~s�q�T���nd���!����^�`�2Dt�4�?\bAҎ�S��t�!c�%l��Ϙ!���t>Z[�L����������Ǯ�E�ǫ�b����h�Iy��pM�ݯx�a����]@P���(*q�F"4�}F"Y@����]�|����P��Ϫ,��-ʚ�Y��?z������O�n����9tƫ{��Ս��L�����D��T9��'zZE�"����[A�c�~����9���x��Lo�
�s�9�s��CVm.�}�C'�Gl��	�@�A= ����|<@%�������<[ ;B8��6xA�r�f�X ��<t|X���˰���`aG��Q؃jJ��Ք�([4i���D��ӑ�6��Ig�8�����t"�m$o7�$���խ���9�����L��v&��͍|�i.s��ކ<��,.~ܫ "�|��;�Ϝ���XU% �;��K�ʊH���}I�N�*�y��������#D3O�f�a��z>�v�J^S��7�L"�����0����~���OX�@�2�d�;�˧I�d7�l2d���y�Mc	6F�b�kK����^�%�����/,�d0X��e_?W�&�ԛ�"ޣ½×a�d<߹#N\2k2�k!H������Ŷ��<�����(ܪ��!���g�|�e����~b�E��o�)6��S�d�nū�V���+Kc�0&�l,2�UxΛ�$3��yh	Mw��9�d��0m��*]X���F���m���k���<�$\."��];Jls=�Ғ�k�,Ma���4���X��If'�G��tE����F4"����^���
����e�1��^Rҵ�Y���xQ��g����O�SO1Z��45^]5h�&��kr������N�4j "�Q���Z�9z7]��ݱ�^�t����
�e�A�,ʪ��vb|�z��W���gB��^�^ �TEWP�j̗���]M����*��%�i!�)�,NZ�W˒�����$3�)""#""")h;p�!�b )dI�i7Tz{prI�Y��x��z/kL�9�.���P_�ιM����c�\�]Z�c-���dx��'9��F;�h"�&c	��{�̏�ax�Э?����a��p+7��,�h03F����,S4�h�,]�C�,z����������gA�ȪlJ-{�+�-��c֊�kru� ��D��P��M^��}�m���k�V��4��`�p�֫���> �� �������(.F"���q��+Òۂ�QR�t������;G���B5p<r��R�[a=���*�����Ά������G�;�'r~�L�߼�P4o���k�C�HF
|۞5]�w`�=@c����'�@E-�!���r� ���y�5z��L"!=k�wJ�\'Uh�L�����XA9@��lo0$a
 �9��L�x�_h�wIތ�*�dp���<�Z�KTy;W�"i݇q��q=;!Q�r���_�(�/�` 5O Z�<N Z�bC��`���pQ�Р��9���9bL�2��8z7�X-��:�pTć�OP����P�7���7�_4z���ګ�nB�Ȕ���ު��Ve�όVeg��Z��	�t��|�e��|�F�4rkHı��R}l%)Dk��-CC1�J��[����.OG�P)��}!@yhw��^8Es��x��L5�-ӳUٳlU�<�Ve�Џ�.9΃�Rk/�#\h2���}p�|Q��:{�}�&cm3�;�������,0P���n���FC:�E�������k���1qh���٪��kU��hO��A@6W���o0��B0�a^wi��^?kw�rv�~���?��}]�y&�����c������1q<�.�b��WJt ��]��{<X[�ƚ}�d31���Y�U٘�lU���*+[��[��z�����\�1�ܡ��=���<��d��ćC�r���8|�zܣԆ������\~��Dt�ax6���I�0;kP����7���Vs5u��%�����-I��:�:�(��[���r�-��ed�1�X�i�R���͡��C_��4}&.=�[ٖ/�%󗻴C�]%ǹ�dw&GȰ"K�!NXh1Jm+=~���+�˫��$��³�r��G��V��������B�.Ś�XkU�v��ʊx̓yȘf���"��\i2Q�����}^�x�{�h���j���~�'���ٖ4-�0��֪,֪,lf��K�k��(H1;g��G��("
�R;��$M.���+$<�,Mp�-+/AL�TU� )�qV� �����t�@���-A+\��%�\8 H�&E�2^����NF�EmF�.=Q�$џ�r���s֖���3�U٠S�c�<�?�<g(ı��1L1��k>x\^m�1=��[���`���UD4�ym�=����y ���� d��H��'OT�¦�� �0m���՚�fMf��H1���~@\z�/}�(jK�|@����!��}�9���~�FNG��O��%�pwm�m�\�4�gdΆr �y��OO�jNu�U(vN��'�����xJ�#�1�@�����/��.{������F<u=�F���n	PĦ�� �)r��$Qd�P�?�ਯ)�C�/�a�4��ͺ�=�'@���̊�E�ۥ+jڌ|6�<�-e��BZ(jX�:y�".���:�B�+VH&tӥ�˩.
�:���n�3<3��/C��D�1v��M��/�SMY]�#� �c��q��h�@o�����/{���#\��%�>b�wG��E�D��!���5p6l@����v?�k�=��ʮ���e� u�܆�mF=v1�b�ݗ�BLL�z����-&ūG,Q2��O�L��?����U�WN�ۙ���5|@�����%�ɟoU���2�6�c��^��c9��_�"��<7h-#L��}Q�T��~f�f
����l�k�'���jٚ�5��iU��G�J�z0�����OFm�XM �T�\�d���t�A[�8@�辀ZV�Q]�ݥ�ϒ��E��+<%	��I�͚�Մ��L%�
�S"��)[Qy��7BH�7#�y���Ng�K�����t�ĭ�U��G����.}4;��ѝ�����Q��5T��/4�c���F cpuF
�O��C;�"���8f��FE�:@ze�P��+�̖b���7����4��C���C�@m9KKv�0�%���=�3�6cѸ5��ر�t2'�iCQ1��^M*�X:Wn�MRȥ�dә�A��鄚�������Z VW����B>�኶�����f��u�U�/L(��(C	���k�y� �h	 	�	�?#_�4�ӑ��x��0.���<+'+aVLk��ź�$�is�.��Z���Z���R4�F��z�1��� �~g�&�!
��8ñi��a0;�x��@�&i�����h	s�]�����;a �J	�C3q������{������oY.{���I�җ�Q�����(��D"2""� I�6�	F�jv��9�R�4I�cXK��s�.̵ c�]PD;.$W<Ts;� � ��}��S/	��	���缨N��>|N��:�,�w�u7|m�/j*�1X���l�g��X�5U`�/���	)����OG�u�6/lV*�Wo{�C#�"�w�E���C��8�)��,Nc����d�{\�eYt-��AA⾽fU��,��s>*j$0��@۲��.�=��I��`�N�JDeT}H�h�� a���H1(Hb{���c)C����u��N}3f!�(P� �U��E2�\i`����� ��qa�����(��l����b(�֚JB���*��I)*���T)�<HkY�tJh��_�(w�l{�1�XP9�92
�t�>�3FP�,�ku�d/��Փ�h����c���s��!X*Α���)��ʢ �
�cK��32.M�k\<1C�$B6�S�/�0�8��
ǋ"����(p�����g"�Uh��]�bա���4��6�ɩO(�/�` U *�p"N �:�?��)��o�*���_�Wh��U�Y��7فga����1�LU�������؈ߍF���$b+�-i�� �{o)eJ
�0f?��ŋ/6�x�X3��r�G�����	�A� �40 i�Ol֦$g%��Y�ć��[U���q�rJ�C"z��<�T}<�tJ�zM��3r��N�w���][#�໶A!��-�SrwE)P�a��HD��v�:<��oqV��0Â��Nϧ�&%N�"MȌ���r�v^Z��b(�=r��C�iz28.Yr1�h����x�Z.�<��L���r�Z�+ls������{�V'tw�$b��C/r�tC�!�c@~\ ��&��A9Il���0h�&dG��!+K��8�硁����^r�|by,C��]�#V66��%ޝv[�ﰖ��~�VkI�]�w�p�� pw�UA����?r��`�#����d�l���@諜��
N+/bf~��@��$r5˚�|t�!�=jo�Ī7�MH�����s��������=�u��g�m��d�	��!A����rCs�	#�<6tٔ�'0)�3#~�ٌ���%���$�OʥSR��J�qHܾ��>�U�߄�3���e-
�H�K!���MK�!����kifu(�`-�*��Ō����X�V�N��r	�$�hΚ-���\tW�][$��ږp؇�=��<M*��XBD*���p�i�Pႍث�m�QK8�/Z����Z|�����s9�N�=�r����-Ў�WJ����!Ӵ�G��9D�x����:ϐgN7�m�XC�):��o���7�G�b��;�3!B-��)���o��%{MH�w���wm�Hj����H����	LUXή�x��r�b\ ���Q!��˙cyG���#G�=��b�L�;�B�����O���/���j���@�=�G"�Iy4�DE�L�M����G ���gb(ͯ���л�C>�ښ�x��s�L���b~�I�~���悔� J7��52�/?x<�G�^	1w��Bh|��b�>�}[L�����	��P�F�}�~{�J���\.��^r(���� ��N��۾N���U񖔂`/������d��UYpb1(\��O��l;���F�~�ǋ^r�Ĉ!	>|�l �4JFE/��)������ӽ;�U;]������cx��g��Nil(儨�Q�\9�|1_���(B T΁L	��.�Ɯ�d-�S��F���(]Y���>$�h���'��؄��^/���wm�|�wm{'�z�?r��CV֗�x�k#4�/7� BK�у	z0E��/N���X|���Q�-dRd>�����C��B�h
tJ&=�9ix6!CC��~�Ӝpv��44�0aSr��T	�@��b^��"����{�3���ш~R�S�4ݥi��S*8ئ9��ZRB�wm��]��˽k���wm7ґ�&<jw�v�L��Ĺg
qlW>7���UU�{��~-&DO�#�3���ŕ�::y�R3�J�"w��1��!��խ@O��y��{ז��k��L�`%n��AJ��5Jڲ$yP&���@`*��w����#����3����7F�F��� ��Š��y����>�P�%�Q��T޻;J��޵=�	�k+t�a�Ӝ�otG��7c�7��=��>DO�E�0�F�K`��d��3�R��:ׅ\�>�7��.a8PR�M��O��A�ES5w�����%�&n�FG���j��Ah,�3��TB-[�:w4ɿ�(�Ӏ/Q�jQ u4��/���_�OXKJ�����|�v������!g[�,	3�*Y%��mvղ�GO]���ފ���˙�9���l:U���_8�沯���ޖ�d���&���5@\}�o�g�̪d|
ob�X#R�J�1����/��&d{���]�w��UK�6��Շ�t�2���gw���H��̓�=�P�8 o6�����%���f��\:!,͝�3a6��(��O�B���"&�Zl���+��%d-�E� ��5H�X��x����K��'���Nl��I�t��Q�|�w,y��e�[��E1���TE$3wEG�� `Qߵ�X:�H\�Ӝ~v�f�62�=m�&8R%Jא���1ƃǆZ[�n��0����P4[�C�����OX��5<��q,�DDDdDFDH����9��-
:�I�K{K'��fS��:�}:�镉f�0qNo<8��Z��1��=��.��#�w�i�bj�P:��;����F݉��NgVR���S��l�7�bS�tr����O�_:�*�ޓ_ջN�	��~c-\�ԙ6�H4��h녁����-z,��A�ٛl�5}D3�Ʒ0�n������~���*Td.*���[u����癔{�f��La�2�>�|E�T��al#2�\B�!�Q�D8�%B~��;�=��"�&�z���ػ�ٱ��SL�V*epxX��� �?��Eg����ՔO�*Z�'�2��b�Yً�0V� ����VU���c!����8!�X���&9QҔ�TC�o盟Q����!@W4y�y����/=���'���E	b��lH�����#�.P�c��^B
IUబ�kk$9$�����
�&�Y81��K&�>����xIڳ�V��uJp��aHZ�hKZ(�/�` -U Z��!P0���i,��%P�_a	��� ��D'���f�p��ē?<�nx,�/p��&1��nh����00}[��mo��5�ݽS
�
;9�0��r�B兜V�"D�� XJ B���F��1%o-��n�����껿��ܔ���dD�@hӖc_6�r�#��#m��i[�m�ʯ,ǲl���ʱ+�/6��;���zq�-������k6A"C@���~�]o=u4cW�-''݋ab*R�)
�����=�r'C���J<���������5s��x�-���֖S0�)�� �R��`
(�	TVv�[K��*Iǻ�=�Y���fO��>2�z��	��Z�0ZTP�������6����oa7��� ){d�9�e���c3��K�2R?�b�^_1�z}]&��+����u��Q 1j'��/�;Ma���ɳ������U[w�ɞ��c�����
�U ��ڄ�'_������~H��䓯�����
�ك8��Qݓ�ݕ���iZ�8UU�\m%�_&�ſ��+��1��K���\��(���9.sܓ_1{�1���4;�&��_��$���䠻K=�w�˨Nv�Ͻ���5k֬i���C�6�6l����[W7�;��v�N�kٳMui��b�lC0j?�|�s,�e���j���~�m��������86&�fM�[�{p��N���t��?����3OP�FKՓ*#fw�4m@����ط���7Dt!v#� �� XY@���M!��)��)M�k�O�cS���e���-����C�TM�h�/J����9F��9�e�_�Em�t�����O����W�!����g�+��6LBK Kζ���M��)��> ���=]�w�'j��T��f�<��3���=�^��4a0��:�c�t:2�{J��%�a@w��<�À;'c�w{���)��Ж��lT�&�c�j�]{�����¯�	[pʇTy��G�1P��O�u�s��~�-�GF/��f���w�n:�(�>�o���G M�?r������@+A�pd�@�f]��� ,V0L�
��HI����.HI���䙒p�`¡4��^j�Gzw�)����ŹT$�ynkI����ш�Vm�\�[�]���v+ja�\��gF�[O�.��V��A�0�������_m#�(�_l3�o��ɳ/�;�E�K���!��HȊ�ɞ=ޟ�ɺ�'{�4$�L�����'_,ƣח��g����J��}��;UhQQQ�Z4�G���i��~Gl_E�ey�jl/3(:�Jo����F� �X��M�����j�Ȍ��{g�mol{��tc��=��P6ٰMu�6Յ,Z#�d�1��f��M��M���+��0m��,���2���6f�ٔ�^ d���4����ݷ�N�rw��<�I^u�@�'L$����ņ=g��e`��>h	�.��%aRT�',؏"2?�/o'l�خ2�OF�#&8,!�wc�Cҽ�dO�*"�.�l��֒i�l����4�0�N��R�ǲ�d��&�tm
�z}ј�^_��T�o�f�op����1
Yg+��^ڰňC'��������y���["$}�B��� ���4;k0��Gy͕ģѝ���3(�;���`ǛWB��t����;l7=��W4h������ʄ-e��'T �z<��J{){�v�]o�:�[+�?��9�����,�<j�k��(�U�_5�j~��DG��c���{z�@����=0�4^r�}��f/i�AG�!�ag�pD��@���c۰m�̨^vA� ("N��zlI�>���$[�N�%�NY����ə6QU���k�V��[[�N8�<���!��h�w2�Z����ߕ/��|�3��z0��3�`�������p�Y���=#f���M��ZYYY��X�Ħ�.�������"���*�Cc,�S��YF�Q��L׎+Ǎ]3nv����q�\#�暮�K�+�(?B���qi4����hf��o�ҳ�93���K3e.��JU)�J��V��-�e�6y^���h���h�`���EN�A�G8�O}qb۲ R?��ԗ���� ��E�����^���)R���s���Ջ��bSM�
�e���,�4����$I�!1d �|@I�T%{�;K0�.Ô�#�s�>$ս~���&T,<;/�M��0�Z�R����������O>�@�_�4��P�����V�gv�iSp�Z1�����/�E
�������(����/:5�P�˘�v'����d=�h��*`�|�CE�♋⩒*�ӕ����$�Ӄ�Ӄ�rbT�W��S�[b�dJI8Q���Ni�t��kJ�'�N^Eh�؉�'��s�37���z.:��Ah��jb�䯔h��iY�������nRj�s���>��w��il�*YC$�\�t��s#�3�G}0�2�����$M��,������--�����@4+������?�FG��c��]���b��K�[�CNn�� D�	d�2Bs�N�?��R�2���6��`#�#���P?i�[|��`@��dv�C���7t�o��^�lͧe\��u�q���_f�5i5P��dJ�V�����>��f7<ʃ����o���h�D��r�Փ�3n�-�A�S�Q�Î4
j L�������b5(�/�` �W j��#O ت�*����{�Xhen��MԷ�����z�c���7�� �{���[���"�G�M�`J��v�σ���-������R�(,G7LI5��՛�����&x0!�1�s�-q3r�1t9ӆ�����R�S0�S+O-D�@�5]^�O���9��2����/UT���~Xߙ���������(V\z�RX��8pĐ��cHN1a�Z^c�� �x���!S.2P�bBY-�+7�+Ok��DxCM
[Z_h�X�l��>�/PSB-� J�/i�.������\W��9`P�]!0�å��c{Nl�C:�n�ǚ����m��wo�8l�w��������#�G�gI]m�f]mg퐷.x؂ou�0�ԗ�?��+=ѷ)�ʱ�O�g&�##K��cE�d�y��a�O��ɟ1������O,D<������M<d\}='��5]W��r�����kR����w|�8�Kg�*���*M3Bs MU��-��4�M粷&�MNU�ڎTQ�ZW�����8�Up(�"�x���랗T����Yհ�+�P�L����'D4�x+Z�k�B'IX<AaqCrX���c\ݡ���ij���z=wMo��M?xƩ�P�S.h<�����9���wr�籦ƥ�G

���䱽���~k�8�lW{\W[\�?��k��{x�}6{�bE���������W	4��]m3\\�����W9��S\`��ꂺ��?f���iߧ)50�o|�����ڕ]m���/F�����%���St�}ߥ+���X|$FY,^�4.)�B���ʗ1ki����z	�V�eC��'�O���	��(Q�C6������C`0���.�uR�/�7-}o}k^��Hӈ^����j�q�����ڨH�W����Aj�c�-��49���F���H��2�ɕ�`��7��$		M��0�g\]eQ6�h�v�,:����(�B�3�ƃ���e��y̳�l�����ݺ��̮�f��}�y��e����0���K��0~�	���!XH"0�pxCB�ހa���ɼL&Ge:�M'b� �@��0����	��6Ctr�+�ל�yu��􃲡IH�ʆ"%�����5.�VsX�A|�H]m�j���������j�%��|-�8zż����Ք,��M�|4O�TzL�x�JO�w�|>k�:�0(ޚ~��<_�;^�E>�{��"�v����4�͂�����qX�|o&����>�xD"�����"��m\��ܪ!as���2\�= ��� �kH�y��C����w�[H֩������
�r�����:G<���QȱE��M@3U�|�8Y�n��b�
�b.]]���]@|��M����ش�"52p�&{q�%�-}�`=Y�Q�@�*E�(#�"����q�
�&G�I��__a11'��� w(&	���(=M��u��X�Ĺ|��0S�o���72g�m���dW[��G�ۄz2�"?|�߽�x����?��C���'���/A#�Ыˎlf��s�����l(m6���yi������a����~�#���������b�w�\vB ��s��C_���kts�'��-�p�GYW[���<1�s������OA�;A��Z�ɖ�����	r,���ocF�3)d�f��4#�g�ex��De�=b Q*�tH�U#���L@��8g��	�p02�\%�8�0�GYc\�뙼r�)4�վ� \���-�������(�j�*�}��k~��_�	�<���]�p��!�u���	&L�8N
�Qc\�R��۴�l]mm@>����/�Av�cR-r" :: ����\'��� "��
 XZ��=:L�Vt]Q6��s�.��~0��7��P߲����j��u�ݡ]mQ�J${?x@��7�����ާ&���MB:�:����V&.�C�O��k�J�3�_��L�y[r���e�<2���W��+��F��"Q6Q�l_3���C�C}E�x�w��a������҇�橜����29�nA�y	�ĲV�g+F��h��m̙�9��~����e@4����/��{����?�)1���^�{!�F�Ǡ*ӎ�U���=�"�|�.�����#���B�0*����6c!� b��XW��l&Q�I@I�@LA Y�J,�h�빦�S7\�u����w�@|qX�{;�ᡓ��I�����ۛ!M��|��A�� >MAO6t�F��;Lz5z�	�e�=��$�UX��CJ�U��AE�)%A�:�R �訡$gEfDDDD$I�6`���c`�R
Yi�°D�rB��D���M�9��c/��^fR��^���n1�:����W{`��Q8�!�@P�%�p�\1a���$�餣��x���&4Rc4�w���>��/]h	Sf���@�������|��&� z��7Z��q�N(ɢ1��o;C��1�Vj�Gx1y.�^��0�|�2��cڌbW�T�J$�*9�
�Cc�DQ !GR�c���2��jmB~F:�D�����)�������ib��~a[��V%i�{(�Û)��^�+b_��0M��DM���xx"W�U�\[vL�oE+�h\�*��W_�MaPc���V�h���1�I/
�z�%oA��,�;�v�
��:G��I2�����'�x~4v���z,�8!d����0Sn7��Î��ݿW���{�h�!h��|w����GZ�5��(�w�_!��{�Y�z4b��� ���Ko�$���l]��I�3�Q=��e�	G�C�U(�/�` MV :��"N ֺ�J0Z�d�o�}�Q&(δ��n���p�sNp?4�
x��Ο�kP��L�FKsu';Φ��C������-�L"0Ti�F��j_��:��8[�u6��+��1e2w~(CG `g�J�� ba�X��s�S�ŕ�K΋Ą�����,�:��"#B�=O��	�X4[U�OO�5�;�"U�r�w���g5��ei����R��b�N\_m��^��Տ묍,�R% �X����ܝ�뭣OY�#O�cO٬-��[5h�p�|����UMy���>�в�= �>��([���Ȃ�Z���iSr�"�2$k˖\�r�B���1�N��ߚ;��-��X8Տv7�:��k�݃v��q\�k �S���y��x�"	�rj
r���g��=���?64�x�FM��8/��&	�S{���]�I[�~nĵ�s"���E��s�W���OY���)�L<e�4OY��<e��OY�-��[�T�\-�gN�ܿ��2CH���mRazJxV��p;"|k^0kC�@���y{���Y i�́��I 6�,�����jOU{����y�']Ѱ5�mq��©�{����s"�ow_Pϵ�%�\�| �Mb�v_�H��sY�/�\�ܽ�Z�����uڬ�_����O��I|��zp3{ܿ�gr
���wIŗ��V%��c6q�}y!�ňWɗ/J�0Sn�eKKc[��$��e`7�6��^8�6lĘ�������T�yy0 Ax0��Y>ԥ1�D��Z�,�+}����en(fs�C�,yĈ���q�(��ĩIS#�Qd1A�b�\.^�.����8CrV���	@��&h>$(f�������Z$�"�퉋N�z�=�k�mQ=۳���b��E�W߉�:߉�Q�������Lv�`k��-����+�ځ�)�zs	�B7�����\����lw��y��]lw���zkw���v�����@v'�5�ݱv?`���8\� rv��`U�bk!�i�kޑ��<��/��/��]hk�f�ؼ{�,o�ύ�WLܨ��7*?������1,#��~-��G�'���ށ_�F^ȣ���x���^��|����#=г���C��x��Ь�����3S��aԢ�f�q�ŀ�a�0�"	��!+KK++͍��b�����,b�h1@�
0�2^lm�� i�
ٌcٞ��kFx!�n��AfB�l��/��T+�D{�k�
��O�A!��o.�2	��|�(OY�`?��gG��| ����y}�2/0"by{D>�yk�ٓ@$d�|��:ȸ�c-�9�\Qże�<mM��1L S��PF㰸�^��"N%q��C\�]�nɍXtC`5��;�����FW ��x���b���mƗE+l�;T �e*'x	��3C*� !8��%�#X���-�t��u�5׼���HGٝ�F���O����-*�J�%��l���ު
�z�3gM�3�1��S�x�⌟����(Iv(�Z@-JX���0!�Qz�*Dfp�%V{��p7JU�[�E�UU8�?>\�B�EO�K�D�Б��[�E��.��T����3i��)�a�Ţ��e���&G(PFWWL��¤0�#g��iv����e�*����M9�+^̎MKB&��V��ld�g��s-:s�V��F*#�w©#7M��ȅ90�r�������r�bܛ���>�Y�����;�f(��.��O�h����1d�,Ѧ��}��;N�I�DdU�u��k�:���;W�}���_���X]2�L@#Z�LĲ��H]X��
S>��,�� ޣ�=�H!z|@ㄜW,F9Ȝ �+����U\m$V2ݮ��v�9u��A:��v�0��H���,�����QO٠ �ϝ��>��C���>��#��6�4�����;�M�g����y�D	�u��j=[,�r; [c�����bs�M��f�
rw|��^��D�o�|� zx����4s>xׂ	��X�+�x�u٢S�9�kF=9��a�	�Sz��-�B5לVO��h����O��qV�u8)�q�<�K1����u��fdJ�1�R�iAh0�]L��ĠeZExa4����v�
[~��=�'�������O,�B��g�8�����燧����>��sy<�}3O�vH�h�N�����m��DV�*V1θw3׸���{���b������RP"��	�$i��!3��	;J���h�����!�<��4�pR᭼`�+�Z���lH#�m5�0ɺ'%��t��7I�:,N����U�b�J��
q[���5�9pӣwl��|��&�ҧ~*Ԫ5/d;�zqr���0>vO��E�����5Ζ�UM���ZRͦ���9�$�Z	<�X��,om�`E�-�C�./7����M�L)<YT܊@P}V�U`�z�*|E����[���Aæ�oe���f�&��h���~3$iD������
�VDl�Գa�4���N��N��^�LwI�zYQ�V8��r�L���8yߑe��l�Ϗ_x �J0GF��zL'�����8��[9��Q��'p���0%���&��",w1�9۪|���~�5CAxM9?k�m��W��X��S���AM�G�⥱@�Q��ۙ��]4��|�zf� ;|u�m;8F��y�ȶ	̈́�Q�%����l�t�4�$����b��&*�0 _�(�/�` EJ *{�90�QQ�P{@�Ӭ�F�= 1x��+!�P��[o|�#��ձzD� uY�h��J��;����{;5e[J���F�Üs�
�"|���W�j���w�f������}��V��/>�X��_�����J׷d�+�Iu;�S��r��qU�/jK�lbI{�F~�0���j����>�`��#�J�(����습���'��R������*���L�L{�1gx-���{ݙM��/D@m�ۜ�����9����R]B�g�M*�����O�E]�36�f������>۲�b�~��5�t��J��<�R�A������;bƢI��f�A%�KS�����m�mW�����>x�fW��:vx�AQ���
��.�)�ХZk�2�QC�M��]��ӨI��.�O����{C��O�uR9-bĉQ.#{"�,���J"Şdc�//f�g�0'^�FJɨq`��
��w�A1�J7�5@a�;|EL��{h����A�!5:氐�!����`Q�P Ix��'��n����WS�H��[�zW"�H�9�Z&.N��1q<�w&o;���4_U����P�ੋ�&��#	��H��/9��*$��(`�v5�4�x�%��Ơ�3��.`���7 ��AX�WX��u%�\c�t��i7�����j1�~~i#G�i�)_ǎ>7���.P�az���Lm6Ǡb�o�k~����ub�z���`�[5gL"=>�G�r�֠�U���!�c�	7[��=�ZHPC3�Gϕ�jP83{ill�w���� �+Nl�nQ<�_8�ؤ�P�#�D̦�X{�Z�؇9	�������sǗ���Z}+��P+k�JW���p/ӯA���\���e Z��6���ڲ-��w���]�]����T#���F�r�,���J5{~��ۗ0�_�fqT�	eĹ�W��eBc�A#n��@��>�i�]�VA�UIfA�0�"=b����IL����h���H�ڄ�̦�W��bO_�ٙM��j�Qu�e	%�]�˘��V\[Zq�o���w��鞹��l�}�����6�V�*D:�/��P��hq\��fv�]�X�e���}�-t Q�3ՓjU�W:$g�C�ebOce�'� �|�ӫ\�}dP{\A���)ވ�-Ձ6�6f�y�g}
7���?�9��_Pu:�&M������s�K5�,՜�L7�Wo�ʉ-�͟��:� ����"m�o����m�X�|,U��/�J������]�"l*�����=���خ�8t��O��+]����������	�F�Ir�02����W���TH�|������U�n�tl����7D�
�u`���O��ϟx�dȾ�\Y���Ps��U3Zv�E?�����e��]�]��o��s��)�S��C$NY�e~G��f�Wn�]���b�]}C�/f�A�jT.O�t;ҴLv���g�-�L�2��Q���4��T�c��{D��u��&>�gr��o��~A�#�y|�+����w�]}o�R����������?�E��1�p�����"�8r�<%
Afő����(���k�M�v%Ar�~�/� ذ�5�bI&��X{��zB�_ddOh{l���ÜW�/�9��;|�o�Wp�%v���e|+ݫ�Y�_��v�����4i]Eʝ$�Fk�%�估(��"�6�@$�����y��+3���Y�_#�����LX�2�`��� �1�����k����)M�2�Nn�=����cI
���� �$���,ecdJ$IA
��[�]%.+^]�x�x4�� �m�>�c���=��ś��^Ƿ�L�UnH�`,�
�(A��H�cjH�)֕t�Ζ�P�U�d��w�FWZ��alޖ��S��\���g���|�����9�A�s9l��'%P�P��;z*�	ЋT6�������|A�v�Oc����=(<��w(�h��W��i;;~��	
�NS�&�V(�"ـ��2�7���h+e�fDz��>���!)w��
Hf.��~�[:�_ⳗ�R� {�y#��%��6�b�5��P��*�Y�j���΍D��c����2���<(����F:�m0D��I� i)�M�C�"�w�[�"0 {n�(���5���Y#'�2��]���h�ً!;NC�VͳRe�d�izhL3��0sI	w�g�kG��˫�)��o,Ϯ�VjT���'��TJ�i��ڢr�p
���uD�ѱ�G�?��J�<�A���I?f�w�d�$��&AXWĩ�6-�ѽ�#�t�沉4H� u�c�9�����LD���
��n��CB����L�M5�!�M_
@�0�<�	7}% [�3*��ñ�(�/�` L *��R@�uB���\ �UtO���wC���w�I=�!����h7�g�S�͗����ؒ7�D��a�L����GZ��'���m�'��n"�L)�����p��H	nZҜg����i9�-Ӓ���Z�\U]gFճ���=�q:e��%ɒח?���!�|y�3�3�+��2C��0��$���a�U4��M��t�k<U	wW�xV�X҈��x7xt7pt�lĲ>g6��
�YI_��U�QԪ���U�2�j	�Ye+ɪq��IN�i�t�a��ïO�S64y*�[�n�b�åb�<��@X3���]:K��� GW&Y+p�~&�݌���Q�%#j s��]���ɭ���c�.Iq3��=���z��G�a%�^��}���P�%օ���T��R]�Ku�L�V��~�긺FMD�E:�õ�ͮ��.����+�מX����}�����	�P���E�n!���V�?�>Murzb��Q��pFU󌪾���U�:�hś���^'Ǧ&�3s��׺ʤ�ʨ�23��5+�m5�G��i��;F�n�hH�Π��zV��Pb�WH %��� �:aqEm�Z�����Z�l����ɝg:KS��sRw�f��.�i���,��	d��z�T2�D1�~Io��ޛU:ɚL���k�& ?�45�I�u��.-���?��T���վRAY�Ly�%�����F���$���C/J8�}�b��i9�vt6Q6;wr�Y�:9�L�f2)���ղ4p��g��2��d2�ʠ6�N�5��JV`T��dń��/�
[X8�߁�8��P�K� D1p�&0@��`09��+���IAT�FFx��,7����wr��������)��Ě�G���n:9�t�(393f���Q��ɨ:S�Q��vc��͙`�;��w�ڵ�y��f�˦M���+;��!F��+1���KTM�SQ��93�O��LK�k%��ZI�ISq�%m��gI$IE�,	y���Ej��Q5�5D��鴝���J���"a�g�
�`��&�׏�9�Q{��(*pd��ݎ��Sɑl��q���Z�Z��9���\�}��J�Nݺ���Ó�R��>=I���\�=E����4t���ƣ���m��]_W�ZU[�Jޙ��
�D ?�EB&z(��P_�%;�c�J�ZU���B����%	�El+WH� ���/Kʴ���2��`Ua4程|�� 5m�>�cC���^ia?�� ��[��.��K�
��Rff���x�:ɾ��������_���*��ʗ}m�䯯sp�c�øѺ�dԴ�+-l./�I/1��&�����i���c��-OG7֪�����"�&�2���+Wx'W�&���p���	|�iaWZhѽ���!A�F�
 ���{pd�#��Ta�K�6�����)+��c�&�1kO�tVL@��ϓ+���eUn�d�􏐕�mtwp`��0MDI��Z`{<ж���A�Bh�@���}�	l�8�8��fO���̨J�ʕF�I�a0U���I�� ��-��+c��i���0Qò�	����r�gT}3��$�^C��Cf�J>�z/$��⻴*ZQ�4s~ g��8�-����Cp[��*����r�-����w ��I�D��h�"�v?�](Ldp�"��U��T��#f���PAaw9�ǣ�ݨ��ȓ�N�M&��F���[�Խ��=[���ŏU�={���z��tC�K��S:�o0���P�7�M����t���4��Lp�ۥʧ�j���sv��F�l��b��?�$�Z6���RL�$Ir�� �(���T!Gs��&)�dc�
�����!^�=�����X-0:"���B�J�0���Lx1��,�%ܗ������t؈"ï��nQ�Y��@��`�[�g�1EkV&9h��ѬxbX�
O�Z��V�=,#�������0
r�?3���
�F�	;��@�S���~�h�<+o" Q�`W�M�9Ӑ.=r ,��N�G�b�� `��#Ѫ���7 �Y����!���8G��-���;**[-&�M�7H1�r�@LaZ��H���P�/[��Z��V�4�uV3)�M��L��pE�p:�&�P�A7�`�8�ͱ2'�RA{'�9v$�.��1�@}��A�%�ol"�,�,Lg*�30
���0��)���u4�>�N�a,g"�
gŃd����8d��6�S՜!��2U��pW���X�� �j0V"��qyw�3$��+*�u������D�)~��ǯ�@}��}׫(��Dl���hC}<Z�
��r�t�A^l�����е|���KŐ���;���x$`/��=CXj�IՊ�4�v�Tga�`$}��4t*Q�l�K]*���gM�/�>��>(�/�` �O z�M0�:��Ч�\ �K���7@�����&���l'��u���C�6��t��CՒ_W���P�ݶ��VZ�m"�L)����ŶL�������N���)����)��~$+_��~�2��^U_eFՒĤNONG�g�F���4�-��ͱ��i�(b��Z *b�_l�8kl��HS��S+(!�B�Kl�	&,9!u����rN�J�Y{|�j��ti/���Y��y����1�,K+H�gYOX%�a%)=+G��5�G��Ĳ���e��%�u�����d���H�u˨z�eT�����8��r�n�<^�7p�L�\��J�2��\�\�̕��g�r�m�ZwW��*w_Y�/)w#��6� B�|�bP�O[Ħl��(JR��ܮ_c报�ɽ&��A����z���?k���2�]�y�Y&[�I�-Q�ҩL�p��K̨*�2��<hbr��de�\i#N��)��[�"v��B6x܇P�d�`,Sq���p���,\�8�0R�Z�=	�nF�˨keTm	�cF�q�Z�A��$]��sx{��l#�o����a7�#V��7����+M�V�1�()w�2e��/B-�K?�Z�!/>�$m&f�7���(��r��8[iko}�+Mf�U��>˨:;i.��M��u^vvȿYr�6k�h���*�0��Q5ǕQ�%�J�K�t��r�l#_z�j7�l'^�'#���X"����!�g�^�mƈ�� �E�1�@S�l����(��\�Ξ��0YS�r�
&S*�j�֪�2��`Ua���63��IfT%/���@�́��Q�>�}�~J�&μ��|�&a"\4�2VصLii��r�ۑ+1	���U�U�]�lFU�{m��xu�O��0@A�X^�8RpY��e�T�1�f�4m.^a����\���9seZ��������er[�[Y-},WUW/Ϩ�.�6ڹ�>#���0���]Tr���3z6��ˈ�R���)+8���BQ�� /�S�۶�iѣ�-F���g�h@ ����o͍v'[�-u@�T5��q���Iϔ6��
�Ns��t>Γ���L�h�trE���vv`֓�i�:�:��ip�=:Hr�}�h�v�l�4Ib�����#Npo��*\N�+Ӳv�2��v�]��ɮv�K�#.[����~F��#�SN�iUb����"��_/��"�i�i��G[{��,��G���h��LS�.8�$Ib�ό���v5�9���A��A�OO[���|
ш݋�c�_XI�e�s�`��,��(f����E�P�l)�N�&F�@l^o0���	� ��E�Mg�r���������r�U_��I�\1[�w2� �Y����áO���*AgH��C�2H�9����	��	��p�0�r@�_��|��!I]� �*B�<�6g�x�Z��X'"�����^��JlФz>|����T����g��|t�G[}>�^�<�����L��0����չ��ɨ��ä�ۙ:���s$.���[w��L^�=����K�>�@S&OLZ�ar��2���1�¿W%Y��m��6��]��7�c>?�x�w�P������vl3:)��8f�����V���GFU���&�y���uBj�)"����%ξ���H !�Pd�AA��_>Q�F�5�BR��$!��@��)�t_��?�O���<�-��3�T:�'�sH����gϡs�dS�;a؃7,��My���w��g�PD�!BB]����4[��-�g>("��?�Lg@m1>��_��F���r9���g=ϴ׃�]�v�_K{�̖�]��^ٲzî���I+�K���Glc(���5
�=(*�ͧRG�8�i��S���0�'�$ɇ.�N��!�9@����i����G�KuiOt\�&�t���`�Ӵn:���u3r�OT0_��H�=�(�6��DME�oXk�I`�uJ���?��ET������$��%lL� ����	DDFR�&`��*; EGtV)�$��#RDk�ZI3���o��x�S�U�hԮ+Zς�$Pd?˵$B
�MD�T{�pl�݈<�:�G&�KM)ȱ�[��dvw�P�	r�����qͲj�,�C���	'Vҭ'����MKxo����:�����څN��jާ��Ow����{�Ȉ����c��dk!����$�t�v]��A����P�g�:�����ד �87���x���Md�S��9�n���d`��mգP%��@0uԔS"�v:F��2�d�e���ks`A���HU�_���b#�EX�gw9��71���}{̶Un5��į�G`E@i��p�ȱL��v��G}�%4����x5��X�Q�"9�T��`7˽"옰�� ��k� �!��;�^���Mb���B�1a��S
��+�3�m�3S�l@k5[G �$����'�9�o�V�;2>�5\��nџk�T+���m��ۓ&�;�����\�]�~�np�3�W�ع4��̶�l��o%!�a��	t�<���dN(�/�` -V J�8"P X�4`J/�K��(�/P��� �c�"�}4���v��aO�bN��l�n8|�`����ãS9ag��M�Z��&D�	f|D����c�մ�}i�����G1ݘ�B�ݑ�0�Q��#�qB*���`*�p�+܀�%~���ob&�k�,3���(z�"f���0C�����jkm߾z�\�H4$��\�t�Tu"~[l��� ��Ƕs��8=�-NkV"���LK�!��>����cm�g�oK���;c�=}1��ܘ}�cj�;B;Y�������e�A݇����E�O���.M^9�kӳ�)�|	3��aJj��#d������.�و�,�ਜ਼��H٪=ׇ��Câ���9"�p]��������ۆ�m@f�t��i��eX��ܼ��vt{o���>�tv��G�N}�q��{���=�^j����#��K�oo��bkOb���7�ƺ���(%�^�c{��{�e~)�H��w��j�˘*�м�j^I37nz4�|bx�Ѱ�BX3�!�6g
]���5�,Gt����^S�+�.>w�\b�����W]��a�U[�l�V͵j�A �nr�7l�6J�l��?��!AK|��Y�#&d�����B�x�8hd�xZ�7eU>��	��N�8��!b�쩯���a}^9�������1�:����S��7*}�p�K�]�0�Z�zRe�-�,(�_
	0��2�hc�^ AK�H��?���ڪ������l:�۰F��[����8�?>5�YW�X�I*��z&8[�tumYz,\3���0�����hq����EWu��9ˑ���u��13f_�p��Z����_�-{XO�H �0	U�%E~x1ܨ#~%��$21�0�*�������$	����â��8uEqDW={����8�mX�Jl�?Df�呃0��qw��ZL>�����[�+�<ƫ����+5��GY��Π��V�}.f�O-fߚ/h'w�*i�p�@ K:���F^8#��7�ZZZ_QD�@���w�F�<����____k毙�t]�8$~����SMakkU^���1�S�>�6lƺbRbLb�ֲW�3^iO�c�_��_�>�v�¦�eJ1��GbH�:+�#�B+*�2�Q�N�v:8��}���;E��P��'9��p'�g0w�,��Dń��;�B ��^q�4o.R�r���qj�1͊�X?���i�`�y�K�.��	��&�	v<UqbSF�Il��VXQPV��a�73����[qDDԅ�xvZ{�_�G�W�wd����
�F��`��D��'}�"�z�~�]��� ���.�4���-/�m��u�JHe�~�#�f���ґJ%�ߩ��,��rc�%�4FU�l�����F3$h�|10�͍�ײ�Da	�y�Z$����E7�
?���`R�mp�ghM��ڋ�2�:'�91����T�r���@��M� ׾����i���#iFr��� FM�0��j�`!�P?�nE/�(�	r�=WN�lqeir�՞K,�>"���8M7��4W�2��u���qiF�w��ջGd�w�d��W~�uј}�4fߔwTw�nT����<T�N2����� �f���OC�,�ؾ��\6���6���R�Q2d�kq�����*�p�hj��M�(qt�oV��:����O��γ�;�z� �M4�-A>�Pf&}2��o���:�,G��2�0��ل��O�9��0+�.��k���5�T�Eu�����t�z�2���a]�=t
�4S�Dʌ?3w�u������Z����s�s��L��i�a�ܵ�z��\���קȇ[�JD-\�Q�a@ ��<W!�B�-�1�E�L� (�y8H�̼���?_��p�mE� �'NK��o�������2f�rݩN'񝯾a]��Z1d��������e�þF���U5d�*TfDȘڜ��*���LQ�
1HX%����[9$�	�<��{���m�C�}wp�cl�=��G�rO�ɄM)�LJ=z��%� �F��d��C��-+�,�&��?��6X��A)V�����aa���RJa��j�A�)��$� � �F�I��ɢ!��_S� �~0qT@�.��L�����E��h�V�z�z��~�#N�v�(N�X0NW!��!����7����$D""#����i`)�P�b�N�$;�#F�0�&��r"jT����9��+��ݶӣ4J�����@���:��<�k~��;��� �~�,�H<��₱ޒ;Yn%g]�ǘq �1!H�M;UO8x��6�Ԇ=LTۛ�C#ܳ��^��0�]�5�J^��Q_�7�k��4Q�n� ��.�{kLY�G���&l62�X���?{\ɀ�?�͊D��
V�]����FÇքe�X��|��gB���������K+��� �~���@$���0�1V__�N�L�n��AUŊI �@0�m��	r7�0k�~1�6!�O6�baE�8H�:�\���z���FJ���> ғ � �κ�t	'��T�2����u��\XYr�+��*	Zp�?���\�2'�쀀�o2��M��F[�N��n�i܌� Lq�@��U��m��H8/�����3��u���+J�����BN�����n0�B$�=�'`�S��P
�\��p��b 7���P��'TDV�*��4XTJ�����WL�����A�(�/�` �R j�!P0Zu�hd9�>�F�'�nWe�{0ҙ��|�w��E����i�=��sAc���4Q�jy�d��-�mdo)��)���<��T2��m�L4�6ykr�%?`7�?����=(w$Y����PMR��T�S\|���%�>�,����̙�xp�z\	�U 3��-\��FT�Ғ�w�Q�27�K\���N$��3y� �d�0�a��e�����9ǟo~T"���H ���T������@�vvL�����H ��� (�-�X r�q��|AD�!��ѐ8$��>�Y�n8�5���/�x�X#F۠i�^�j�j1��:�=rU�x��*�V��RWF�
�5b��jue�j�����U����nߖ�b��~v9��e<�g�{�ȓ�MKTTvѻ��*�ꋀ�4�\��v�@��I�V������oz\W������Ȟ/���!�C�{�\� �X����[S�/p��ɋ5y�X&���6�d�;W�>�R��/�h:����;�$��p�H���ب�J�"��Jѓ.�����NN>��e�4�n��� �aqHH#}�n*.�h� ���^�#ٖ�d(�d,O���t:�dF���O�I7鴹����ÔB�"�	����!�iӤ	��#���dXC�.*(d��f��hH$
����k4��b[�3�θƬ�J�Ho��h� ��=����Go7OOmr.)-�*��fj�	!���i�O�J�ѹ��y������fs��N�<�sIf֨c���kLR
!��#���k�J��1���qC$���R�s����^�)������m�wZ�s��;��u��+��������ּ�J~�(�T������Pv�~A0��vrUF��S$U@BP�IM\�g
���D"�[����wZ��8n4mN�&���ȶm;���6n��pO8'\N	w�۸�A2rq(��8�����Oe隒�5-�����ҩl*�ʣr��E�v���xI�ɫ��O%Wϒq��?�"5�<-ɀ�SH� �=����lّ>U�
�A=_i�A��D�P'�	UB��-C�(oQ�i��moe<5<%q$��o����5qTr�؀�M�M�.�J p٢N���'�Vi�pb4(:O�ދ5�R�����jG~�4o����4������������L�j�U�g?���>�=v.�֜n�]��W�46�H�Vr�U�޽O�qC(ۙ�oy��.���Xr:]�:?�v�=_PO�!P�h(��z���=<:k���ت�1>1���<��8.�}�תۯCa;������n�;@X���},�m=l�,�G��x��r)o�Q8I[u���+��m��??lS�d��y��k�+��Q8��×���0���*�Y���:
)*��J]��Cy��t3ʶ6�o��I�=/���eْ�	(�z�j���/=kG������m��%l�O?vDT6�������V_�Ż+`��3�`/���_��}�m䆏 H�@3�%Xd�_���'�C��b��'?E-Omꚞ�5���6��ɍ��I�$S
<� �X��xzXͶ�3SosvMv�������LՋ���6�ICZ���V���J�~�	�\���Q���I�]��݄� &cV�X~��C���.@�iI���Y��G��`Ei��'Vp��C1	/X�t�⪏@����>� \�Pϗ�7����+����5�i�3�n�t<5��|T�媎f�%E���cH�E!En��M2n����^d��9�8�F�i!�HĻC/̩���֔�����,m�m���F�Ws�D�n�C��/R�j�G�;���/*����O����kt��鱻kَ%��=�;��Ţ�1�bz�1�K��*0h:�"�%�p�L1ӵ�ءչX���$zX��j-��?S5e��+��i��/_���x��H)fw��Doˋ�^3x�Y[��e|�+Yl_��*�'Tb�t ��²��2�
V��C=�a��0�,���-��i��dV���`t'M�Ds������խp��7d��@�J�Be�5�04o5Y,w���Ѥb�	D$I��1� �1���(�a����g;��>7MS:��.q㙿q���I�d���Q��Ri�����~@7���F�N������0b���\�O�L�zԯB��@O "�j;T <u$NYM˛B��Tqm%_5��E��]j�8.��p��z Y $�ժE	�p���1⏌PL�#$�����Igϔ�s�?�C�|�Ɉ�Tnc0E�/D�}M"����^E!v���nt���18$�5I�U]�C��Tu�~a?��4��T�/�X!Q��^��c������OɞJl��m ,�I�ٜ�bPP@�
�6���&!\�SNKȡQ�6[k�M��ȷ�7]@H��:\b����W�T�/����},Δ��~R>��І�)�x�Ż7A"�)��Gq��M��dp���/���k�p�c ����o����KiI!t7�/q���A|*�&*ne�S=e��9J�� �p *�fy��i �����`�в���8(�~�8��P�«�(�/�` �Q ʌt O X疓��a*Y��܆��jD���G�.��_3].3`M��Ю
/���� ,f���\�����\���2p��m��mJ^n)S���I�{�k�`}���eƖX�]�)Ռ���_E/3g�Q����e�����(��ZG���UƂcE�>�<r�꺞=`����x��@���\W��6��0�y~Sۧ8�=ܾo�p�jܾ6%#vnߝOf���P.u�v1����[;��by��u���3�IvS7�o���_͓�����7��*]��C��=!�M��H�F��π@?d$@�H�B>p`� VD2��c��=߯�;U�J4��N5�� ����Q�Fu�ey��(ʃ�tQ:!�:�W�����xu<��/ʓ�k�IGO�S�Q�٧��=y'O��{��ɃdI����>�mO�	d��^��#v�5&f���t0��v�nvs�r���XI�e�"��&�d�����q <3�ߟ���[L����	SR��oRQ�HAͲk��mxL�P�#M�d��bd+\ ò	-B\Yd�Ȣ�|���8$�,�󝧺�ȣ�K4�:ӥ�uaySU���1ӡl�J���a�=ߣ�DwL�}M��0^���z�[�P��%����|Yai���X"L�P�)"cÍ�H�8MX3�ق�Kd��!���$��	�ظ����e�]�k��C}B��FW�ԡ�����5�dd���˳�p��+ܾ+�w�-�k�|~2Z���R���j!�_��
j�}C��cX�����o��Q�'T�}U����b��q�C��kC��zҎ+����i��v찥�\�"��Vv��fI��ȱ���n�4}�}���r��`�V�۷���i��Ӌn��H�9k�����RT2b� �g���#���/[�4LJt����h�/�+��!1���+�6ܾ������^�G����T���t�'-�	`����/���	i�R��(&H't�:r� ���U��DeMZ�yr�̪��V�]�X4����x�y}���\�������O�p��'���s㨏O���G�h#�!=�����Z���j�a�R�e�e�g)�������#QVn.��l���*n��8{������t�/�v�����G{�[��@���o�Y	�Cax��bm(*2Po�d5T���n�A����Sn��E/K�l�"f<����!1W��0�r
���g�������?ڿ��s|1�b �1k5y2�^��O7y��Q ����ǀ��R���d�
̑)rHdU!�/u�]Q�o|���ܾ7���hO�O���Sa��Ҽ��������19I�#[�~����h�0e?Ѽ���ѕ�s�.�8W��p9�|ܾ4�/n���?�<;l��J�݈*9d]���	�M����Tk�tED��Ȓ3=,L�%7��b"���)M(��>���j�N'ʥ�m+�N�}O�������������ÖQWt�n�p3!�	�@�LG�T���������5,6��\�Xآk���s��e�D��iM�����
�ꦴ�������;���^k�`��>?��'tj7\tG� ^��AsT��2H�[��-����AԔtǣ�(8C�oƋD�Q`R(�҇�+�9�Х��.6���$6DN�G�UM9�@wGS���FSW��<"��_M@��0`�o�7��۷e�����[����2����ϏgP��+O4�,T��J�,�b���-�8PT �%P;1IAҸZr�e4>`a�����caBL���Q��,kc��eS��B1���}��G;�I��_���i���~	2Q2�̢����BFV-(!�@%u�Ɂ$�R������9#zJ��htig�<��u�5g4~o&�`4�����D�o˫!HnC���s�x�Q� O��a<�G�[�x�_��B�Dȉ#�Qe�G�DʌL��N������8q�X]E��2�����5ul�,����cB�0T`�D�J8�X�O]��Lw�(�rډ+"VMO�t���M�Z>Ӌh���4����M���o88����,��C#"""2RR�6���� 5Ŕ�I�$[��NhCq�A�����u	r@i�L��ܙ���n�4�b��>�3!���Y�]H_�m��IG�6 ���i�ڃ�p:/�|���?�b�[��AKm�2��o��^6���'5��U��YN�O�@8��:��|Q
퍡*�Ǥ#��+������Z��=J�_�$F	�����R�!�JZC9�-t�.�e~+������@ȕ(p�tz���놞|���%�S"�*.�-�E�*Y�yP�l�RC��r2�YP+g՛h��V ���1fԩ��%;y�]֜"`����?W9Az,Y)�iõ�ZIr�����݁l҅-mB"R��~j�e%�;N�vhF��h�Cvj��-e1�\�a�E�\r]8�>�S�p�(� SZ���!�-��� ~��&��F�z�&g:���8�`�u-Z�H<��ZT(�WAD�z�Z�[���ȗ#�؝�aB�v���J}ф��=�;|�]:";Jm���v0!����<>����b����U(�/�` �? �h4@@�mļG�BQ��Ԍ�p����J[��{�G����h�-���F,��zBB�JFFklr���V�al^M��u�Ey��/q���X[.{�B���b�O����2b	��(0xn���ġN��*8]�6���J��mGW���pY��(�h����������>��AX����9@ZOLƍK�b�X�$�'�:P�N�%�2riU��A��X�Udݕ8����:��ǋ�~�����ᶿӛ�?)6{�w_���S�~?z�ܦ��"�H�鞣pj��R���-Gf����pT���|f������]��rYwu5q������N�+&���@�{�Ý�u�X���	n{ӎ�m�b�tp�u�
��R}S��ꩾՌ�|�y_�{�ۼ�+��h����+��I@=cܺ'9�]��vQ5*xȇ�0�!ٚ���6�͍<��F��@�Ѩ���9�Y�^�t���͘�RT�A�1��|�X�ǵ\�H.�)o��|{��j����s���	��4�U���|{��mhgٙ�g<i2�mqe�@��m%f�H��hW��FGWVT��yj�Q�(Յ��a��bNwQ<��<����vLb�S����E.R�H���.�!W���4pcWڎ걱@rQ$�]���E�5��k,�����Rj�{��]6���,�#sk_��3}ݻ���~n��;�۞���|oj�2�'�����%�������LqTt��M�E�8��ZW�5�`��,�4�R�H�d*L:�_I�"�;b��Y.��/��y��t�n53'�!��j��H���=��X�q����|���M�-M�4�w��뤶�ڛyl^	�v���si5�|������<}|T�x��BK���ϳN�UA�D�TЭtЪ�Q���R�����@FP�֖&�m�]O�K�(�c�lQ�.��4�.���|��7EU���R��SN���]��jn�ꣂ�N��p۵U��C>�M+�m.4��Dr�H 4Z���5�h$�Z��ͭMF��+��&8ս��My��:^<���/��� AK�3
1��BN���z�A��d� �<���D��(0(�R]u�*r��N�K��|���7ŋ�����R��6~�����l���9��|�n3��N"5�s'�X
�?�:�{)b<�6�P��۹�?�8�v~A�g�0��z;��&�YB��s<����gH�S�������H�Y���LHs��X��������y����W?��G��޴�����`��f*�po��rDY�W����b��c��#F��X�N��?x>����S�^@ŷ�L|��"���o3����l*��m��P�g�.�<��ߑ���'��O_���~�c��r}���ɞ�v�K��osD:��n{�b��.�v�<:yUUT`��!ط����&�9���37m�Ya�����m�-��+�?�I6g���j�y��W�R"��R��P'���K����n��2��8��42FR�������� �5NZ��$PbXد���9��4o�T������ hn0�����Oʋ{})ɧ0c���f>�:������xźZ�5�@.��n,�-n�~�x,�}��7z�w�4�����ͧ(����)1���y�D�&�e	���|� h��/*���ل�wC���=<h4�P���Eo#H&�7N�&1+&Z�}��8l��Ox$<u*���f$�N�7��$�F�De�����^,�}7#�˱��GD�mh����Ibw����°h��>��#\��S��'�1p$��6'�Ò�l���#1F��0�XQ�|��>O`�R���	�N(��_Cc���,ɡ/�kL��#�L7I2�&�Q�>�u��'�#I`2$A[�#��y�����h���>����ε�	�}ӝ'G��r�ģ�4�Ϋ���gV(�[�W�X����F�C������_Բ�D>�,�	�&���Oh���!XaJ�����
kn��=��Џ��f�KYBH�]�Y�
���W%(�
(�/�` �> 
a�CP�Q�5SS�P���	�8�ؑ�<�)rƵ
2�ǵԚo`ѣg�Y�vk�<�ۡ�Tlq�1�����{K�AJG�����zE���.�B�+ˈ<x���HE��q+�E�8;�L{�ϴv:�m��Y�oE���oK��������p��C��ö�Yߖ�oi�|�w�**?x��v�sb7��(�*
�A��F<C����Ll�P��&@P ��_6��y�gvO�Dbe�B'Շ,yK���q�2,Q[��;x3�gZno�q6�<mSI|�,�����p�s �n������� ��ӵ��?�{�S�X���*����v�ބR��1z%�*ŷ�?��Qe����-��2��+bK2�p�Q����X�˗������|T����?ՊW���YU�A�ꟾ�4�	��J���`8Ոg�z���Tӌф�.�t�:���|Ed��^gs���Y�,�����nu�-�k�=��=���@�n	���iT,F��N|#-�_��V+䲼v�"��F
�T���a^��@��\
@��'�p�Ud>��Ὼ��ኸ	�W:���	�~Vq�?��V&�ZԿzkŃ���H�W���-�Ἒ��oy�Q*��U��1��۝��*��v����~U����7�ݷd.������#�����G�6�V�u輜L�_�|�2� �W2��2$��q�Z������r�5;���k���C� t{t���B�,d*�+��~������8�����Qy�yj&��}��`]��̚�+��zY�4��?�S�[S����P��U����F����&�U����e_�=�K��gk�O��p�W<�F���Z���}�=1c�5L��Pf��Iuڨ9��S�}_�q^YK^;J�P>%z%��	��4�q	Q��ӧM��ǐ����By���U����i9V��1�XH�S�{�Y�Y���Gr���x�
� ^ͮ'ɟ�b:
2�䊾o���,{=wI�+�;�����
uE����=-�p�<,�|��"�/�p�]�p�.�8�$��K�vp�w����V��߻�X�����i|�8�0> %�x�Z��~i����ws�8'�H:�&�5��!�Ȩ��է�_G`Ʊ��`r������\�N�y�&���y+��TC��*��V��1{�V��P�ݨ�"a%�@���@c�J�l'�l�H�=����j�-z5�*L7�.4�����I�,
Ӹ��fT���0����������ɹ��FX�)	nw�Q���I�j��{� N�j��*K�~���}���;���GJ�v�VoF�sy��O����ML%���n�h��ӛz����抙8f9͊��QA����Iۜ\q�砭~}�v�c��0�E""")(H���� �u Á��I�L�o����V/q�4Yz�]�^zZ���q-ܠ.�8e��}�q����g}]��/Ɯ�J��7�yeB������(��jZ�)N����͉�Q��K�^�Rģ��r}�AJK���i0�&��҂������`�&[�d~�� �L�-|����Ͱ��{�ɳs���_,(��V�g��"��h3;�O�ځ�'����b�>�����J��,N�b������s��T���`ק��Ɋ/vV�C�q�����C�3�+�;J�8��j4�����K���VHQ�I�b/���lfI��+��˽�!e��P�_|�}�$3��yTfq�����{vly���e���Lξ�wU������q+�X`�/쒔�B�␻��She��<� B}'�̹��{�>��b��iVh�h��1_�R�[_���TT�W۳e��+��O����ඛ�����Qo ��O:J<�r�Sץ���Hup~�I"t�}�_*��O(�c��M���9��e�^ZҲ ZWm?�/*�(1����,�~�D�W3@������?�'i��
J��-L<�d�#�%��0�����?�WqR��)\B)��(�/�` �9 ZWFp�Qc�/��T�V�a{H��s��"��8�p�;���x	��r��h�g�i���5����{�-e
*AgU}����� Jj���S��ڱ�?9)�V6�?�٧��ͷ�S]�{d*E?�͍<%�ʢ�m(w w�鑞,�ί�Uu���5���Uu�@L	y�5�χU�Ϯ�F���.�mD����K��k��G��ܶǾ�ع���b0�6�eU��ڒ�?�ɞC��w\���j����V˻>�N#~[��������<χ�<��k�!^�-�l�
�D��j�C!�ދ�1۶����[�J�±�ƭ]Z_�;�Er���W}��R�ܶ��w����r��>�2s��*A����f��A�aW�@n�=p1	���u������ȇ==�w����	�	ಪ�6�@N��.Ǫ:��9��Q+�O��q�	>�Ti�O��X����
���ƈ.S
 ݪ�"�,�R"-� ���	yUPQJJ�!v�+�%�nU����u�뱪��~�XUYU�.�%[���.ohI��ZLܭ�c����RE��	��� ��m���g^��5]1��E�!��V�f!�r�C�y>�(��'�M=@���)	�����h��x�ɉHG-#�D�η��2]L��g���W=�}9I���XU��XU�* �xi�>?q��(p���n��i|�U��n`�������ǧf�7n�dUm
���?����"6�K��C(,�znu<�su�>6��������d�C����T��{���pz(�U���øy��0zv�s���������ɪ�$q�@�7��(<�F����!.su���8Iq�=�$�pP����A�7�$酅�Uq���b'<��,�����YU��d�!~;d;�iǪz7㚹5>�3ҩ3m{-Xu��	�È��,����>�Y�V�c�h�m1�O��^+ÿ��3����ד.���+�r��������07�Cv�[�G���1�2�,�\5W˒h.�ceT��3�<!�Hx������t�����޳Z�@5��cU��4�ޘ��گ�k��=��
�Z���<
4Z5nyt���:�rᾸ���q,����c�c>pt�hZ.-3_.6���¤{���z�߃ɞ��~AI�[��uw�u�����)���P0���o�J���Ӟ�rD��w�	���X �sM8�q�b��Sv�^e�s2�X{�f��D��4""I
R(��)V��c=Б�I���7uL�S�w4�I�� n�6>Rr��(����\����PH�e^��W�>��qؓۗ�}�N��b��,>�hb�`JuY�A��߀D'/[@��ޥ�!m�M8�Ғ�;�M����g��-=�>��|X���j�E��p��aF�ѽ<*�}���>�t(���-��29E�p��[L��a�n ���<�'���"�&��(�ly��
��{j�4s���$$�݉Qӳ�DT@����[r�/*9����|Zyc��YϺ�Y2����/���'2��,%a�um��HV�n� =��� �O )�hR�$)W��g���V�� �ס�P5-I�8��n�Y�N�ߙ�^�I�֥���j�qY�Ù����8��b�J��4��掜ܹ8�M]b���K��/(�qct�V���D+V����C�~`lj����d����w|��J"h�����s�c�q�I���c���$J�f���Ε�NP�@8NI��M��j�N_L���K�$+�b���|��b@�jNeo}`x�9 �4d'�T� ^�Of�4��{��su��e�w%ɜڛd|W(�/�` UB zg�=0�Ng�0��_���lƹ��~�r�O���2tG��c�:1Ct��f� ��40-���&e
S[d�������u��d�z��^�o|���k�,¬[̱gLc1��?��J��j�wm�.��LAl�r5?w�[5�A�ɲ���n[�ӫ��<�ח���d޴��[�K��'o�N&��{>aK�&D��v��?v��w��͇�W~��O'�u���ޯV��4�E��j9*�-�<���>�o��׫�w�H���mK�#��`�eґ�/�n�
u�)g�$¬K,��a#��$�dȤ¬�>I��BM��#�w�.�ll�{�w#��\H�w<��0�~���V7��-ح�����Oy��Q�����w�珷�dq֯�w��~���r��|N�������S_8���YW�o�}�O�+x�Y�h�kr�� �@��~�P�]�����fPµ�,o��m]h����;ׯʾŨ�����:~`o�`�|��pA-���ޓf
���p��M�S$N~}�gM���w�}��i�M���w;\�����k�{�?�p�[��/�+�]��Y��3�A�_��izϫ���y��g��ｹ���?R����T���C5���􅟷��_��G�%8��*�2��fD�qII��-�����oӅpï�nt�@��kT�Ά��m�B��F�Q�m���ګ'g�A��J��1{u�Eb��Y
._�?�� �ez�N����<n>8��#̺�&̺M&̺̱�ѪJ�o�7owm%��a-��s'���\&���ق9��%�+k��Z��Օ�	��.�P|cG#�9��p�#�Z�F�������0��,#K�o��Z��{��օ��}K}��|��3>�nJ�.�_~U�/���Q;�5��Ի0�pA�~�G���R��(o(:���0K$5aW�g9t��c@JؽaQ	pZA0��I��넀.Q1&��>�av��ږ�|c2���D2�K(�|�"����YW��.TU3�W�TaG�b������	�q{��vڞv4"�5�F�|�ͷ��5	�`�|�����->���s.n�%u!���0w�8�V��i��"���36v`_�ޣY��~�Vױ\��9�Z����2c�㒉�Y��h�?����~�F��z
 �n�h�j	���7�{�[=I�r˃=6[�,!����^�����2��P:��Z�
�����n��o5��6��Sk���p�+_.o	��{G��i%�	kȖ�;ke:2+����(AwaN�ZҜE�)^d����\��^&�����~=X����)�fyu-�3kjR-��Գ�I�iG�P�A�v@~O�[=�N����v��v��"�Ӯa�cq=�����؂�f�������:�{Վ�VGW.M���8Pi8�6���'͖C�x0A�n�G�&��0�Ë..�,�U6��L�,� cF�������H�(P*��'�!F�Τ3�s��8g�3""""�$I:�G1�� �r\:D�$�ҰgaG���%ʫY��zg��(p�%����������h�7ヅJ��S�r��(f���m_Ės��Id-D������J���x�ay~�	��H�'�T��F���S6N��s'�mE�"�N��⒫�>��&��W��a�m�Z�d0*2�8�#�q9F]�;S٬|���S��=��-��CA��>���>��M#y��!9�� ��62"��p�KMA���_�>�"�����B��~�q���y�X*P���f����k.�.
��K
"�G��l9����2�
�uİ�������Ε������I���WnK}�&��J:fxݤ�.;�x��;oV=*�9���,h)2��v�LB:�H�\�M�χ�oDz_���/�~���d��a_��j��GE�eL�
(\(W�G�ųL{b�ODM���%�Rx"r�@�)�����aɉ�9�GJ7]\����*g�}i�p�Q}�\n�Os��)���u~��c��da�q�S�*�GW�{���:��4]s�
v���M��y3�; �l� گ�"i�� }�[�>���I'
~(O���z 8�0UV�������%�qA��0r�4t��q��(�/�` �5 �K`F��J:@dhq4�X������	�!�׷|+�b��Ls��\q�@!�#	��Z�������{Q�Gt�7�2� � � ٩���`���?j��)ֱ?��/���]p�N嗖���;��|����4����E5�3�5p��3|�)���`�S�H}�C�9�[�H��^��9߳�&GiiIX'NP�ay ��du�)������Fa����4.��1E�M�e%��#�H�X$���u���_�`�&߸h[H�<���ń�!��~L��9��o�%>�߮Wg�����)CP�"�S����@�j0�gn��-f��q��C�[�z�q�H�����#�b��R�k0c=睖r.=R��d��+%�L��K;T��U^�O��~��a�u���)�B)�~��5��)��K���q�f���*�G2�c���5�x�ui��Y��ę�jSJ��(8a�X��Y�#�b�jXv��#�P�B�����wtm�H��!5�ie�>^����� VKh}<VݡsuV]��L�̽�sB�i۠�jrRF�%��i�Zs*��<��.V����W�S�Ofv=zM��C��r�J��ޭ�TXS$-P��G��	%)����{k0�Ⱥk��qW��hGrMȉ3�j��b??~��8i)Uw�ߢ�x��_(����dz�ϣM���L�;[�
���������͕d�:�\]����ل�<�9OH��25��F�:�Gʃ��R�Z�3��T�}��,"��U�*4PL6�?�p��@l�$�s��	���E��s��@x�nݷkc���$����Ѯ�*sr��v-��uVY�C1��!���Nϙ�+�mi�3�M��}�1.��jI��n��c� Je|�#�y�I��Ku���<�]?v傗���&^d��ޣDԲ�^�\�;���$��tv��,��$u��zJ��+A4�g��s��䡎�9t�,I�[�\���A�Wu=�#b*�HL��S=�@8�����{�x�v	gLMe;����HQx�e���=��4��!E)u��,	4""I�B:Dq
�����p	�k��ov�T'v��^r���]n�B@�=���"V��D`+k(��x���z�r���Cc}�!W��
�.$C�+�0�G��h!�˧��vaS��y Dx/�+�K�Ҍn��x�� Ĩ��{+���M��Q{ɫV�B&�BN	�*N���O������}+5�s�
����(��ۭ}ʥ��5���>����Kr�6H��yH�!O�9uQI
5C��X\y�����t�/`�ߝ���ԵR��Y`��a~����݂�37��\=h6�HL�]�~���sS������$�Q�����H�dXz~X���q ����7{�a�ܚa���:P)~�l��\�(�����O�Ͻ�$�:~v�$��6b��HH�d�-O�d�M�>vM-/�����v��;���g�����z.AzpxV;���}~�M��n��g�A�������f]�O�uu��eFw��f��}[��B�mx~��%�!�|�;����rr�? �T��2ﾞ��iXQ�.�e@8���4�2�O����Ud����q��(�Ok����<~�=.�Bg�D�É���%�i=�s�`��]']��s�t�w�ѫ��@�O7>�x��z%o<������5L(�/�` u; �b�<P�I������� �B-����8,��do%�P\^_�=��F�M �G"������2�HHKj��UB��G�\���<�Vb��r���Q��J��Z��jU�I�8OoKR ���:����,�^2S�lw�r�-kS��3άzv >�7�s�٘D��ꚧ7O�7=C��7�D��{�ޜ���؈���6�NM�y�I�\�kk�/x.	��"�� ��f
��,A�!n�؂ފ�=��>����lC��D�O�\<u/�pM�{{u[U�^��s�~�sq.�2��7�@ZX��lA�;�<�O��ؕRn7�����`}�.��'\+��kb@��*f�V��n��ۢ,�펲�� �ȕ���oEU^N���������BO�=wo�ߗg&J����{�~+��.5���5E5j���z� O\4�G��hQ���*v"��?�EO.W:��K���VA/���4h���>���}����x�/?�eBS4�eSjU}w�Ǫ7�Q�S�ˮ��v+��VT���Sy&�`wB�zy����V6�G���,���콝�:7ħ��I�Ѿ�-�N�����i$q���S�Q�#���N�v7Yᴁ7�VЃ��G�X�G�JX��ʬ��r�zЁl)^��ݶO/��{(�`*�,K�Kޯ�����	���3=���a��ݯ�=e� �У��~0P�t�F��)�oz��e@I�F-�d^MK]	��>��i&viBnUU	�=�ڱ�&������c���p�[���@��u+r��{u��B��W��x!�����(�KtrU˒��ZQU�jjǢp�g�M��T^Rq�P�r���tZ�oJ{�&��%�*�Jx��ޗ�U}7��8н|�����/�(v���f�A`����EW����%t�8x>|i:7�|�Ֆ��*�Fu���%��g�.��l�U5M�?��Á�I�;��>��y��[ӊ�1���"Wht����������*���1.n����
ϠU�<w�/*���e"�U�ݨ�]Q�F��u��zA�i�=���u,}(�ۭh���W�C����,��/���XQU�{���O�$���v��u<�O��$��n�+Hw�j\Qu4+�)�rE���#��-�݈:���i�z��{<��xz��K���	8����,/�y������!a�v@��M��˛�؝� ay��;���67b�9���T�7t|�ǲ}ύ����m;���;��ˋ�U��h����3?$>��4��>���l��}-g���!9F�8�������ݭ�7�N׸C'�j�:�N�*z��?��h4�<�H;�`4�wk{,���S�$WT%}+��NϤ��#��@�E"�$I�)UX;@�a���
HF��2
�c��� 8f'��ѫɈ_��79���|z9���4}�����A�j���?2J���ύ��y��:#Xχ��gMfDb���d?Kƨ��|�-�['~����m���7�� �]h��� �GM��)�yPB�]v[�|FԆ,R̭=e�K���
��'�(����|*ǀ���^`ȣ�˙�P���M�|Yܹ���QT������*��ʂ��Y���У�	{V1#f���-tKO$4(�!7n��c���.�6؝�����,,m�T��3vE��[� ��-����LH6�����Õ<PyW�VBO�R.6bV��.�:��5Ւ�y��v��
�n	�(�#�"~]3��6*�����`������Ì�ƈ2��X���Yއ�����_A��\6-cކ�	��il4�x�3�AD0�`PE��l�7�I��d�%.�;���dE����H�J�� �"l�N�i�tC<��"uL7j(�/�` E Zt�BP�NcR�Hx��^۟���.�?m���	������{M4��ɡ������,h,10#{�-e
z�vX�~o��0 �詵j�:�@N�����J�=��)�/�Y�-��lj٢@�	��8� ��K-{F�kw.�+� ���V��
�[Hc���	��P;]���+ސ�lP��R�8;�w��ڪS���D���:�5���u(�;��W� X"��<���퉾u���o���Fy���
�Х��Xn��VUew@�,�:4?�=u�\�M�X(f:��T�@����W����|��|��[TK^���C��r������R/������(��?�6�h�id��]ir�N����F��q��@��t�����S�g�L#��b���A�r߾/��EL#[�u�d��X
H��mO�L#�[�^�~��r�����=��轭���yhK�% 3�`K�@ ��
(bbY� �������A�����#�N�Љ�tO��L#K��$�8)e�}�h�2�,:�3���8���77�m����K��Hx�'��NX<����Ł_1��ul�{"�������W��n����o��)���?ڜ�:W�����857�kJ�y���b�,%�{ｧq�`���	����-������c�Ի�P�o��Ku.�����h%=Ł��~ J%�~zo9�IOq0��C�����[O�{
^�*%�z[˟��)`}}T��K:-�0s.52��H^�P]Хn��6�K�Z
E�ʄΥ��^Y�R�.���?�<����J�����idS��]*o��C f�?���%1�,^��xrC$�b	hv���� �V$z��SaS��dP�lcOx	�J�CU 	$U0t�\��ap���G�N�����M��H��	��$⥳�x��+|Zɗ��̕���Z���։ZG�^������lj2!1�ö�x�JC��v��*����ލ�v�K���;D�A�37��i,�ǧ;�XPE�"Ե��$�m���H�f�,� �R��`�*+eWȥ)��2���D��I�Ut2�����n7�]��h��]N;�a\5�<���(���dK'U�,�
{�u:)�KZ
(p�}B�O7i�����+#hO��0"���%u��ں�RR��Z�"TQ	�&P���ʂ
�FI�YתJe�4�(�D��JZ��{��TޣѦqL5��qi��G�2|�r�b��=�i�Ƕ�d:6���L��I����D�`{Of�o�ړO���[��U��z��]�lW�ូث��L��a�؅l\��LR�Q�x�e����֪T*r�.LE�ɩJd�ۜ���{C��exK��)�g�1�͌M.�M�&�D��Z�T[(�ܐifJ9�L�cSp5�²��RߒlqJ�5�-!)�<K�Q�!��8�7�����ThoTd�Z2YA����R���ؔ0��bK0���O|��q҅���;�Uzj�Gs�*�T�C�&n���Q�'����� ����Gz�/�{z������#�]��z>f������o7�@_�4������%���^p�%��Ɂ��F��3��fZ��No�21pU��t�8�,2��p%��q����Z���nwn.���l������o��0��f��<��5""#)HRQBQUy T�(3'$)HRh?�/Ӵ>%�͕����%c��w`H��xoi��NL%�Ͻ���Ad�I;q�����8@��5���L������P2m�	{�f/�gZ�KZ��y����\�H�}�W��o�u����CWP�6|��=�(������E-%{������ܟ�mr�7�u�����|����cc3ltڬ�+��$@�d���F�<���3�##-S��BG�*���[ȑ���-hE���TA�&~nc�����q>��HG�o@�^�8��kM�Ig��	:����92�kR'{t�Er"��n�Z��+Z^��h]龱R��� �5^�l!����}�����	F@�L�J�a�.#<�A���q�´f ��q2��\o����/y~�k�;�{�p(t����I&�K��g�˽�P�Q����F��tn��8�sx���f?J��>� )�Ķ�p�0d�w����y�f���nġ��7B��{&���}ю�� u�F�����Q.`���K�lD���w)Oأ��A��(���z!7s4$&�ûȻ4��(�/�` �W J�p'E ��:�Fq�b�?%:�<����
��L���"����"JǄ�ڊ���c����U���e
���?���@~�><>���7��yb^���u3^^��v�z^����x����r���D��P�(e���uN��|"{�"��$���`ς)`_�Rؕ��I0	v��M؄�	���` ��`OL�ű����:ZG���ݬf�*VO�4�S9�Z�j�R/�]ʥ]K��_��}��ЎBOh��*� ���A{�BiǠ��Z��3)�v
��=I!h~���S{�io�R{ 5j�f{��������6t'4�������������˘���q��{wq�3������,���7�TŽ�m�k�����݇}�s�u���d&s���(lu�}d����>�h�_�����lu{����R��K��n᭳�a��QG�d��H=A������-/���W^�:�[���-��Z���Yg�F�.�p�YC�i��i��/���X��Oc��&K�ԓ��d	�=�O{9�-��tu�������
gMe�Wc歱�2֗����2���r�s?M;�^��74��R5�ԑ0�2���)��	��$';Mq������Ѓ^76Jd8%�� �Dd)���)6Zn���*-v��D��1A"��@w��ټn^�2�����n_�(C�r�*��8;�D71!�67ZN�[�-�-̍�1-�T3�M��`t���T�D����\������WƳ�#��xrd�(�H?�F��<�'��N��:I'礜�"���Ef�p���7yEZ�U$�&��$�T�^yD�I#��$"�H!M�g�4�]�C��<$W*�$�@"�#�@ɧt�"ٔL��J��Hy�B2H% �%�y�|��ցs�"���"BCH��2���u���1C>X��a���0����r0j�j���4h�D�(�� ��?��B���p�s�2Hq�1hҀbЉ��Qz�R�A5�U/h�OJ�&��KUD�Ѕ&��ݸlA����d��n5��������r��8o�g1����7���p�����]u�';�꩛��
��%O�K5m%Gp�!���P9����M0��t+d���G���nt� ��la�p���,x�ϻi/`�ti�����5C���H� o)Op����R1�y�ܑ�G�@�x��X����(�b(��{��H��z|�G�Ǚ�x�l����.BDÁp���7��;�a�!"$�!4��.�2t�@���P�dH�����ԃ}�0;��3C0���{��p�V������	T�L\��%�m@%�� GA(�f+�B���<b)��"F�#$�_��Fw=��*�Q6j�5�k���<�Cy��PZfjB�|�W��Vj�c��*�azAk�I����JU17i��0��J뗍�H�n*S��!�r���hV��l��=�2��
/z�T��@C�cg��b�.�=W��^\s2�����u�z�s�`.�b����Q�yʹӜf%w�u�p
;�	�0��|e��le#W���?�i)K����"g��K�vrO=���� ?��#{�ȸ��0 {;1΅-���Gv3��n���S����-ѭ�F�CS�I9�t�1��82ܠ���U#��j_6�Z��S�͍�
M��j�Vf#�U��I�tU�nIJ�1��m�������+*9.���NWe.��Vf�弼nan��S��6�F�#��j_6]-7fs�M�%�Q�]������Q�I�ծr_O��p�1�tTL�0I���TmP��
�v���6��EU��ljl*�)�[�&70�1�i1�ԗTW�,jPm��+Ҵ���@Q-�,Ia��� �#���}c�[�`��vUtD������*:���s��Kw�
Uڪ�ς*M��g��ϩ����>��!�}��w�J���b�}b���x�}�λoθθ�k�/�w��w_�N��W�f	����Dx������ݧ��/ʻʀ>���Z꣍����,iEgH��#r�Z��t��toi.oi0��[��Z��r��f��	������O.�&�F"�6�K7�KI��ri5��]�j��������xa_^؇'þ���2�;ʰϪaUþ4�����E�ëË���E5��<E=���(^����x��yb�Y����U/�P���ʪ���	���t<:������������
�
o�UxSx}��s��)��e���h�]���4	�OO�<.ϋ����u�2/O�xc<������i^�����4/�MG�6�h/��C3���%y2�C��T�'��Uϫ��/ϒ��r$V��82Kwi�+�PJ:(��+%K5r��I?i�h$Y�}R,�t�#=�ԓQ�wҪurN��Eb����+�I�H*��n�MN��|"��M�I&m"�H���3�T3ɕ
T��!�4+_H�%�r�U�J��AJ^�^�2*���E�H��#i$��)����2)�8���1!S
�	
�$)VP�24TV�J���2���;ے��#�������ej-8��k���?���7!&��k�8�HM��X$韣��~�s��`n�!m~�sW+��?}ܼ]a"�7�h�ޞ���
LG�ï����->hM�Vo]�fmi=�E��Q��?1��t>-jC�<
qO�PL�{<��0i����U��>H��X'y[���)���?6��	�U^P��z�d��z� ��8<J�� v+�����r��0��.,=vNZ���� zo;[��O:�(�/�` E[ 
 �-B@�Nc�:H͊F��-���nt�(�vZ�0}�O��G�Bm���Eߙ�l��i-���`cm�r�R�)���[�ktk�ktk����I��C�N��_��\��R��Q�h(Zt-H��Q��d�Z���`}f��¢��h+���h�	� ����� �v۱����;���e�f�>bY���X�A,�'}ͤ����>���FR�S��eͅ��Bp�A�����?8E0�A�?���?聲����h��tUH��Q�l�����~�Ôւ)M{�Sz�J!k+�]_��J��RPh��h�&�m=��g���r�Yn8����c����\,j6�G��M�n�L�L��L�Y�{X�yX�wX�sX�2�}�r�Yn�[�rǰ�a��r���+,������Z�;�rcY�+�me��,7�垲�AX�lk)�ڃm����`[o��5��,j���ڂm]���`[O��%��,�(��Ee[?����O���n��Kv[�n-7�r�,w˝ˍ-�ښE���VDQ'�۟r�ܝnsʭik@�K��t�H��t{H��(�ʝG[�� L)j:��r��8�m�2G��P�7ʴ�n���P�iT�ThT�/*�*�:�Z�
�nC�L?Q��(�(�L�i��4�hi�(�#��Hr+�Hr���|$7Rr��y$�*���{(��$���Ɠܩ䶓�u���Z�lG��#[������='�n#���L��L���j$wɍFr���q����~��c$���#[������m���ڋl�E��"Q�I�gu���k��n�A�o)�}~O�W�[�~��o#����b��췒���O�9}ZI����?�;I���{H���o�ۍ���OC�=�����h���`�`�|~���1�=>M > :�ݩ��a�q�n9��D����{�v+�� �$��d�٭Evg��T�[< �Lv�)�3G��9v�$xf  ]���oX�1��0�̺na]�����k/�h]���P�f��7�ڑt�H�V$]����H�h��3�u�\�h0�K0�`�	v��n8���v���1�M�v��n-۝e��l������U���vO��&���ݹ��v�݄hw�ݚvi7�Σ�m�����#���[���#�U�M��Cٽ'��d7��Ne��쮓�dd���#��d��m�.�u���M���`�N�nź��!���s`7�V��a��vw�n.�}�p�h�Շ`�v�݆��^�v����4춘�ֲ�Yv�n_�m+�]e������n��?�������M�nw��)*)f
��6�n���<��F���m>r)����<r[��D�=��{r[On���Tn���:�MFn���1r�Mn��@�O`@��h0 <�m����`�d�m�m(ضl[�mDp{�������� X������\p�L�X0� �@ ~��S۴�4۴mډ6݀6��6�D�V�MiӁd�k��-�i-��,�i*شx �9����mg�N���6S��?LM��0u�\�{�¢�@�D�w`�f�AA���� �A�;�:��vp�:�=����`�\z.=���a0���vS���^0�fs��0��<��j6���U����`Uk��1X��j
V��:ʪ���X�NV�"�:U�R�tT��:���#UÑ��j4��L�n"U3���H�J��$r�T`[��rؖ�`��`O�A�sP�Tn9	n)��l�4�t���t���}����`_k��1���k
���:ʾ�����N�h�׋��D�hC�'}�R��У��k:��zt�4�z
EZ
}�F���HC��r�k8�5�D�M�F�����&�5�z�|�D�N"���;Z
�	vt�΃���k9��<�|���p�6�Mf�mX�V{�Ֆa�aX�Vۅ�fa�UX�V��6`���6���v�R�:���jG���v#��Hj'�ګ��#��R�(�虜Pjב�t�����"��Hm+R���N��]R�KjoIm-�����~ �~�i7�������������@�6P{jc��M�6�-�/@�	��p�7�Mf�m���{���a�a���ۅ�fa�U�����6`���7���w���:���~G���w#��H~'�߫��#����(���P~ב�t䷚��"���o+��N��]�K~o�o-�����~ �~�}7���������	��/�Y�1�M�z�Yo-�e���������U֛�zOY�&��d��lv�z��w�7��k��ބ�w�ٜzk���&�i�hv�f�7�f�7�ΣN�Q��hv��n����O��&�4�4uZu:��#��Ho���#��HoUz��Pz�Io=�'�S�m'��d��='�n#���N��N���d����#�ݤ��4X�O`I;�%�K��m�.�y���M���`�N��0ożAA��&¢�@�!��Lہ`쾃�u�{t��f��j0{fπN�A�0�����7�>�& � ������x6��ڍ͎��p���]�f{���lj$vi#vi"vi!�5���emfY���yX�;,�6�̮�a��l6�-fWǰ���j0���]�®����l���β�X6��f[��*�Me��lv�M��.�d�V�K�j�����l�6� ����m����لhv�Ks�Z��4��.��Kij=45�M�Ki6M�GWۡ��hveGW������e��7�ڍ�C����(�'ʺ��f@Y3Q�
(�t�#���\�H�F$S����O��Gf��H��Gf�٪�&���ޓ�z2Of�2�Nf�ɥ�ȥ�$�6���d�F�^#W������h��3ru�LmFf���o2{��v��bd�,��MK]�:,u,5,�,u	Xj��!`�/���`�N���V/�R�e+��ԏ`jF0�"�:� (�/�` [ ��T+D0�Q:�I�M�M�1���� m;�Y�����.<{�O��I�z`��ZP0!L:H��6����$e
���@��g�p�o��*�4.�q(]����'��'�O������4އ�rI�|��=4�F_���\C_��q!�I(�'�P�LB�FB�6�T�Q��(l#v
;������(l6��EagQ�X�ztz4z���.(l+
[�]Ea����(l(z�=z=Z=:==ډ���r}�|�|����x�ǁq���}���Áq%w�I0~�l�*��*逯〯ု߀�݀��`\O�р�3`�x��g �%(���Da �R�@����z@9{G�l�w����ېu!��9(	y?�z?��|Ҋ�U<������'�x�!^|����Qxq&/��[��z��ϓ>ɔ���#��M��M��ȸ���������-�ZK�[H�d!�W2�B�U�d i�$S�D�*_B�A��~$�*���L@vm7�=�v��^����a��l7��>a�M��)�M�vC���ہ��n��o��n)��n�v���|h����A`� ��	vǁ�,����v/�n8�[	v���I��v���j��4`7���m�I-�Of�|��h^��{�׭��F�u3y��n}�y�{)����Mv��j����ޒ�-d���^!���O"I���J�Yi��f�&ڨ���il�cl��l��l����Fl�0{����bc���{بZ�2<�]xv���l�]�EM�m��<?���ExQ;y�<8+y� ��|ۏ�6#��'���W�6Un�r{*��r;*��2iWڙ�ׇ$Նd�t2�VR-'��r;)�g��,��%���QKI8Gɠ�$ҫD:����V�oֵ��s��>ڦ�,��}f]�٦ˬk2���u}�6-f������:�����N���ػ�ucT�zJ����;*�2��L�Q�Q�t��9�z��L�j��I
�H]��M�l1��0�i,��&�zm��n#�mM���utMlZdڊ�t�i*�S<��xK�y`&�u�yT����Ë�����<n+o�ʋ�M�9�ۆ�bN^"G�\/yrV��<���#�tU�Ɠrv%7����tr���[�f���d�.�K6=%gKI9G��'��*� yȩ�v���v����l2;������N`g'�=����l::{�����Щ1Щ/Щ+����"8�8{�l69����e6�d���V���*�ͤ�1P���
T��';�'�'O������O�^Zػ��6���@a_��+P�_�ll<	�K��@��O�Y؋,PR-��ڣ_�����!{��?��f��}�*�v�n6N�q6�ucWifWIfWy��w��9l���q�����}�e_k��X�u���}��ƫl���Oظ	��q6e�#l܁]���v�I�����i���5���6���l�#�������N`c'Yn�[��=���(w�ܜƦ��7厣ܒr�Qn%5:Ic���eh�	�h$j��F)w�]�g�[c��t@�6��a��<���ǭ�)���*����-�9��q��3xw���^�`�x���InF�{Or�I����*�g�=��R���P��+�n��[rl2�%�gH�,ɍ%�~�ZR�7�t��hY۱���u�ͲfB�3;t�z�-fY7 �5�˚�e-��J����/e�)kN��k��k.D�����(k?X�Q�J*��
��P)�2t� �-�����D�f�B#Q�5HY灬� �d-��ۀC��k�⭹��f*ޚ�xk���q�G�D�8����Ǻ�c]�c���"t���X��XKyHGyBCy�Ex1'�uOi%�5 �u>֏$kF���d�'��*YS%�Y��J�R�:�5T�����M'�n��[�Nb=C�Β����SR��T:J
�$׼�5�f�o�m6�ݶc��v;��f��X;+�A�Y��,�c�h1����YɾV`_#���mv[��Z7j*%Z�p~�v�ۜ"��H�Q�Q���v��C��趒F�F�N��E�]��Q��D��Dc�D7Q��h�H4hM���<p�
n��n��V���!]�C����i)�F<@�&�Q��{�2/j/o��ۮ�moy��t�}�h�wl���R�:�f�^��3�t�uz�:-f�:l��ƻl���m�8�a�Y6.��aظga�a��`��`��`��`��`��`��`�l�Z2Y-#X-ا������W���R�:-�x��a��K�DD���O'iI�lT�:�� u:���Zf�Nc��+TK4�@�n�N3ѧ��Ӛ>��F`|����ˁ����60��F0����P�:��j`��ƻ�q.`|G��j)�2
�%P�'��N@-���+0��� �U��b��F<�E<'!��?�C���)�OK,OK��i�^��*/~�{ʋK����/��{Zɋ+��yq����^$���	�'-�$9'�n��r2ޔOo�%�#��#�i2�EƣH����Q��'!�N2�A�-H-)�4��f�۬�mmǶ.��i�Ͷfb�J��3�]f��l��m��ƕ,�X<#�x>`[��X)�n�v��u��9힣��hk?Tj&mG[���Eں����n,���v3Q�HԵ����uغ
����2غحr�皈����I���Z��Zoy�������\WyR�Kx��<@��\Cy�ExR�3x�J^k ^�|��֌d�=�ZOrU���ֳl=����uTJ�'[C�kWƹ�㺕��l��Rw��3d�,�Kv�Q���"c+ml7k��r۱�E- `b�H�s=���Ǔ�e�W�dt.^8*gY̬���������s�|�r���Ǥ)�h��t9�=7!i�f����p��EJ}ڋ�X��z�[`q����r�z��d���[z�E?z̡A�mQ+��$��I�%�8�v	}i�Qۧ(�/�` e[ ��D,GPn:7�n71�B�z:�K��6�ьS;A���|� �@�ɔ�;�n��|(���;����9��w�3!%)S�����E�G�>)�.��X\�C8�A��pI9;���8��\�M�?.�$�����>�#���I���m�L�DzM�f�I/I��$it�5z��ڢ�lk%�h-[t�-��-˶f`�^`�N`�F`����v4�MGS�Ѣ�(�bTi0Z�-ڋm��D�6�DQ�}���奮¥�¥�¥��RK��|�yqN���wpr~��n��v�rt��qF3��8O��,s+[�I��H����5Y4�4(�V��M�:�,���l�Ԣ�˶�|��&�Ķֲ��l�"�5�E����,*;XTn���`Qy����E%�E��E��J���l��)XS&��D`O��J@��)7m��v�+m�FO9F[�QT�������)���^�Whk+�\�5mmD[��>������ҍ�J6^�.�[*�x������'*�x�l��;���W\� �)p�{p�wp�r�r���X����K���O\�N\� w�&��@���;eך����uuΓ|�'�|'QK!���u�u+�����#O'O�&�6��(O�G�^��\#Q�F�f��\������D%	D�.����ҩ�s٥�l<�Ƌ�8�����N`e#��ugo��l;o�����bT6���Zh܋ƭ�8�ѥ���>��I�]��.vn��{7{�mGqK��M��?nv7��t+e��qr��\�(��ȥ�d�I�^���ri%��!�\&.��[qq\\7��8�8�D\\]܏�e�L���x��c��)�ZJ�]�x�q'�Dn��m8�Uᶩp�S��-n3�v Y@��X8��g�p&��w`_Yx`�uNG�jN��U
7��.�ǃ�y�ك�������Wq�|��Kq����O�_�^98	�������yҎIx 	$_�I���N·��.K�R�c�W.&�4$|����mf�����m�n��Dv[����2�vil9��~��nt{�n���}D����5^�*\l*\�)\�-.�n���r�E�6nv��}'����PnS�ئ�@"I�m,l0;�����v`a����n��®���(l5
��ϡ�U
ی��Ba��`�y�gv��.�����88�U��)�_��>��^�`9�$���ni	�ll �t��$�@6 	����I�[%lY�^J�1�0ᯄ����d�J¦!aϐ���']�V6�u�eegY�E�,��ۨ������P�HT�u-D]������\9�?��Q�^����k%I$W�v�ֺ�1��BW��>�����8����s-�XGq���&n��c�Ĺ^�:��_��$k7I�6��(Y㑫%�ZI"I�S˚�"�eYgY�E,k,��l'�}��&�����cʺ�&=F��}FYѤ�P�^�����6�F�Ѩ)�T����C��h���.CM9��C��О�B{�	=e���n��Yt�}=��B__��C�����A_S��R���ZH_G��2h�45ړ/�k'��}��W��M����V��K��$h�4��:��AMyDW�V�V�y�l�vO�����i�y�u<m4O��i�����M�i�yک����x�7^��gJ0ϔk��i��h��Eo�o�O[�����i�x�.^��Źx�//�ŋk��^^�$�{]�r��I!���>��כ;�̥~��J.��)��^�qJ�q�m�)�p=y�z�דU�������'����±���gJ�����c���V�iq�N��ΔE�j"����i��l��_��t��Qj�ɣUi�0�GjwIm,ҔCiJ*�=%WKIm�'��v��$��d<({Ҕ=@��5�u=[�K�:����8����o�q+�:��m���۰�kX�SV��U-��eX�0�j6��&Oa�(l���M����K����g�Um���������ƕ4��8��Q�rh���.CU��q.���Zu}E]��*��uME]KQ�3�k!uE];Q�,�� u���n��T� Ou O5�g��y��/���Ӽx/��ō^\��f^�̋�y�ԋ�y��x��x����3�j3^\����x�/^܋����s}�s�9�J�����8���.΃�3c~�漂�` 7gܛ%pm&��q�3�D�;9g�s��	��r�h8z�=�
w4��)��[��R��My4���&�����F�l"v6�����2[��]:ۍ�^3�Hg�����l0o�O6�B��8�+;�������T�Hj�Ym2_b��Xm#V���Ym"���jYm�j�Tۍj�Q�1T��j�y�k�&��f��H��?qNrp.��L O:����C�M%�$܌����-��~��mXx�,<����p&7a�?/a�,�i�o�n�+|I�J
o)���(��¹P8�����xp5����xp4n��7��d\̃�<̃���a<8�Ń{�N��!��V\wJ!�f(�'���c 7���S���C:O�� �4����H��S�RH�n��3	�#�]�E§$܅����'	w�2d|��Y����W�O
98Cq�'�5���u
�s�<Ǔ�;铱��DeO����)�}=��K����$���=6���I;�u�M�fMifMIf��Xm2vu���ƽ�)�XS��$b����V{h�oXmvu�zʮ����v���]��j�v��Na�QXm&�m�j��v	a�I6��=�8�`�l܂�S��#�`���kJ"kJ֔Cv��j�v���Vۭ�D�xԔ2�^Rm%5�K�[GҞ��=	G{�ҍ� S:��K ��N<Q��ll��"��9�(	E�Z2�鑉8����%���<j�Z��J�"�y�����]�e����-�L�N��(�/�` �? ��pR`�ݤ%�}���gJ?_�;ڕ~w�C��q˝4���"WC����醑x���`�̫BҸ&bۍ�	)�&��8����e<�����an��H��G��1�����6��:�n�o7�[L����c�u���%w��a|I_"��ƃ�=B�<�8
a�(���.!�MB{�v!lB�B!l��a\�0�#�_�-���	�xa܀6N�	aӄ�/�g���8��<O��(0~��ƛ@�L��� �C�<׀<� y�y�y�������}�._����ű|�/�ű��O_����ŧ|q�/���������{�G��ō��E|q">��d����ȸ
� �d���͓q0�_4�E�W��q�5JT�Kh����q��x�sP�m���]�V����lRvRʾ!eר��Q�{2�3�������[� �'�Ƒ�grv�[�?浦T�!�r��ax5�
��Q�;z�:�Q,��Fk�5��.ɞ�RF���,$����E���RJ]O(/8(iJc�B�I�%�
�� n ��#�z72癴Y&{��̙;� �j��ם�����F�V���Yi���S�Y�A���OͱSi\�73�����J��H���T1="���*�D5��T�G��Qr�#��#�J�u$�?�Žb��*�J��od�q�*���ׯ95J�Y�(�H-J�@y5y��uOrqI���Z&��i����j�z�VuZk�gթ�����iMK�U�{H�>J��Ț�H����B;(���R6��o�鬹�J{�ZK�j�洩��4��*@qe�=*��]�0�uUNq,����#������"��%�mH	�}�H;�m㣭����l���Ťm/�u,uwI�\r���w������sg	��y=rWaW�����7��ɳ��E�����d���S�ة4\�Ff��K%b%e2�j0R�L�FU�DezG�Aq��9%���5,�?���ؼ�f�<R�q�y֧�o���T�U�IN�X�F������:(ie�:2��@g�a��t�rwU.K��*�^��զ��U�����(��ReP*��WƑ[��O�I۽B�WٷF1T���)u����Fs�'%�G�uQn���q��3�5�K�z�3�:�����`ue��d��R�O�G�I�{��W �5-��問r�����>r�Pr�P�pP&�@֑H�d�{��W+׺�Y�hJi/5����]��	��S�|Q�2��r�4U�x�QK�6T���^���Uj�Jk*�N3����R���SFR��:5�6��;���C�r\��GL5Vv�(O�U��{`/Nz��;8��z.L}��G�-����6Zk]��k�[o-e��%r�ճb��6r��M���(v2Nq�6�C'�鞓8�$n��}[�6�nM����%�4�Js���<�i/���q{�^֚v��ڲM�&̻[0�h[���+�5Vcŭ��M�n��ꦩ���5�_-�,o�u��/�v{�����nM;��4k0W�
t���M���ךfw�|��cv��^m�W�j���������2���mMK��2�F�T�D�a��zGi��7Wƕ�P���ujM�:��}�ٴ�]�;�Q�w4�9j�5�)�}>(���M���Y�&�<\�-�#k��B��AI���,��I�H-Ƒ+|#kx&�,��K�<�}r�E����5&��H����������@KR��c�`	&���Di��3__��Vܨ�$�5b
~}���X��c�qDl�e�q��%4Ʋ槨�*$!
��Jw}C��7�E�BM*Ois���қ�0��u$���t��Q�����u�.rw����"^��+���G����Dì��L��ml7��Xf`,��y�G�F6�>�ȓw�ګ�<.�)O#�q-�Z=s��,,?&�j�NhB����pm�;U�Ι㏾��>a�U_j�l��b� ���t ��_u:B��Bh�W����Um(���ްKUM����*�7�q�%3��y{�'e��x���B�(�/�` O ��X$D0��Jr��F49�ҕ<���:)��a�|c/�^?���+?T�g��*`5��R��/�=�Y���=i�Ƚw
H=>7�c	��d��"���3y&��L�R�C�e�u�.Y;��!�$^X���Aj�JB�.f�G2��$WJ�G�H,�ʭ��ܑ��J�|tVf��o�l4Vb%���*]�U�h���Dgf^��%�7�P��@Lh.��%�������Z���,0�3���>�(n����E�����!�� ܃F\�n��L�v��\�g@�a@f ��
KA)��`�)��_x�UH�&��v#|�*];�FN�m��:��v8,��/��Y�a0��]�rW��;�A9+;9'W�M��Uy�-9	Vr`��_g�X벎�S7uS�:����z��i6c3N`1c��-~��-�dI7:ɋN4؈F4�]�	m�G�3���g<0�d��M<d!b!��@�2���`���W�:�*��!x���9����~2��Nr w"{�x�U�QG��i��ؾ��eӟ�j+&6#�'��lZld�ڞ��l[ڸئ����4���M?l�Iب�Ķ�w���`{-�F��f�F�Y�A��g�����l���7[�̓���f�Lۡm��f��ڬ��b{Ҷ����m6@�S���h#�]�m��mX�D�8&�@�gpJ-� �a0���p6��C1�o@/HuwEcjk�Ɨ��Mm�,�+]����9: 7�F7����`��8���~,vb����a�岜���5��r�u��rVC9(��SrX'�$g5؁��_�u\�#紑3rOk���8��<�{�pz{�Ǜ�t',-KZ�>�t��^���|8�	w��o'vb��m溉�����r������X~��t�a�V�r��*��c*S��)O����\�'?Y�>`�؀u�d'��&7�j&3��IN�6�Y�u�G>2�F6�l"����w� �^{��G���:Ǵi�px���L�t�����R)��M�6:EUTFg����uP��
��>���O���)W�u����M�\��i�cS6���t���XǊ�h��94ES(�L͔�ʴL���8'V�¹/�R7gCk��^�����渔K�΅��$�~P$��v�#NK�T�]P�(E�t�"\V��U��[N�Q��ګ��rSZ�2���r��RRp�RR:p��QN�Dy(�\�,�Q�P����N.P�I%W�vɑ�9��$�rkRMzo�MI��,#qM�)i��d$>F��X��a�ՙ2S>�E~�R{�^$�1%�����"�����ǔ�R֖��rv����!%�����Q�Q2�'�+�iD�(�>��Rǅ�P�PJW�U�ӃrPj(em@	(����i?�'q�'���|�O�{rO�.˲��'���y2OR�'���w�N��N�ɛ,�r����\�I9��8'q8	'�~�o2j7�DB�&�$�krM�V�j2˱|:M�ɼ�,"��<�R��2i5�$�R��1I���7�a�"�Ȩ�H ��>�9���!w�C���CJ�C�7���qHK��KvɩQ��#$���ܒS�eX�x�� ���P��LIv=@�S呬�H�I"Y���5�B�� $k�^)5��OweWJ͕\�




���ʆo�a~���h�e`xkBM���d�=��];v/����ʊ������p�8֖��ف0���p���������`m>�g��=x�a�X����`�1ăW��;�c;h���@�!^]� �'�	�� �=� .� �8q8�~�n0g��>�cw��=�z�3���u�f�Ԙ��4�TJ�zR'�kEUԭ�Pi<��:��
*�P�?�Ӫ��Okܧ}��=�ӧ�h�qY�����)��4O��S<��N���uZ�N�)��xN���S9�q��M�4�6mӨٔM�^�5�j���X�z#Q$��=�;N�4�1��i~�gz����&S2�=�c�����0S��*֨��K���6��5��v/�R�e��:��P���K�v�.T��B�f�,T��B�V�*T��B�����As-����^��J�4���kXs+�һ�VмJ����4�����4���4��v����J��(���P
�����@���@k#P:�I���=������N:����Mڤ���Ik/��&����*)��IJ�`őIi�������Hq#5R�T��4��� /�"�M�D���R!Ń4Hi{ի8�)��v�*Wi���>ꣴ{�G�n��f�k�ѧo�F��Q}�X���WuڪVͫZ�\�R��U����l�6�Z{�-gn��T���D��Q�$�p�����!5�H ��3����}9L^��Nf��������ck� ��)�`�SrX��{��X�/aJ�F1f <�-�'PF-����X�| �ʣ��Y�&D�R�3q�D��-��=<T�6	�@q���@g�3�x��3*�F$[w�7!��aMg�z������1@l�陽���(�/�` U6 �Z�9@�M:������rE;��� �	��ꪫ8$�c�/�%j-���.2t����qck;ɝ6j��+K+h`��"�R�Y9�+W�����ĈV��u�2�>+9�p�2���^�2��>�袉����E��+�\����}䠋s��S�)�:l���7hV����-.!�F:4��^�{Y�壵�{&��W�á�`F�Y�*
=���s���2�B�ìXo�&K&��V0h��I#m���`D11w��6 ����J/t��H��c����aG-�a^k��^�E�e[�m�]}���2��>��eβ�����^۵\{�Tո�%��NX��Z�k��ۺ�]W���mEY�U�����(*z����():����((��z��('���퐥[![�v{�&���QD4UD��"ʈ6���#�G!�HT�D)�=Z��QK�5��M[�][?�DQAt%DQC���h���:��t�d�l"�H6GǮ�)��#c�����h4^*�)�)B)��?a-k��X$nQZi����C���iFۖ癤l��DR��ҵ�y���<MӖ��g L���O?2�N�nnO/�M揦!���5�๳�8M6fڠӱ�r?�p�q�&2"�dz1�^��Ӽt3�+p#�p� ���cs��Y���C�pW�VUlYվ�������W����j�J����j�
�~��j�ʩn��j���b�����W+սJ�N��j�
���G�QmT�E5�*��*��WUC�P%TUA5P�?��ʟ�ɛ�ɗ\ɓɏ�ȉ�ȇ\��g����{����]���x�����]�s�_U�o��k��g\�c�_��[��W\�S��~�&^�$>�o��������w;�_��W8�O��G8�?x����kW{ڗo�5[�۰M.�~m'�1�0�/��.��-�r,�r+�r*�r)_�?�p��]y��]�����O��Y����jYmߦZSKjE-����V�bZK[i�v�FZ�u��6o-�ZB+h����|v���<�g�l���2KfǬ��`��z�.�e���ͲX��Z�*Ke�����P��:�&�d�l�=�FVMC��1�mAߨ
�r���@#�����T�3�/ْ�\�{��I��H�d=?�#72#/r�9��wl�uL�w�c9�c8~�/��-�b(~�&�s[pS�KpC�����!��Q!�@DAAA��BcaL!�T�`c-˓J)cfdDD$IARTP(4�S?�jV��J�i<5.g�Ȏ�7��ԓJ�*�.��>B��:ۿ̄�T;.��gs�Y�	�m
��O@��6d�rś����Y0�=�Ñt�ɱUK]�Z��5��υ����M��`,�UC��_16٨��e�w"%�-U� �˱G�@8��g���_oQ{�͈���F��n
~�`�O�t�����`Z)�6�RD��1�`w�Ƞ��*�$ǫ49
O�����B�+6bi 7HQH�ȝm� �r5G�W�/��_�n
�*��T'�<m,#�d��e�M='�����[�"��Ua�M����L�r��ә�m�Bܲz���.�m�
^�͂yI�����#z�D@�'¿D��,��+�&:QZ�8'���%
��l3L8��$?j�%4p���1�G�������3�7ț�!F�yT�<�a!I��Z0͒饆�h8<�����\e���WB�>��b�bɸ�Tli���]@��B�˄m�(�/�`��C ʆ�@h�d8�6�6��P a�XD��p(�5Έ�<`B+VN�l�AN��ʐdp�xCDJo$��l��L���ߗ߿-R��"��+Ҋ�"��L�����RJJ.�Q:����t��r�\���Q&JA(�į�
O�b�_�½����N�E�� ���r.�r�v�wp��<�58�g�_���*��Tn��\ʣ��k�r�?��7yO;�)8����es��]�a��]�'��zOr$���ȍ܁7�"g�]_�
<�#��o؆k��4��H ,�T�a#ք^`�	1�%�ឥe(��9}۞S�%����'􄝐n�H	)�@Kx��P��")!%�"'�$�e$<���PB�GH(�H4��-����?0׹؊�@BX�!D��P.BFF�R��!XZ�W �\>�8{`H�ݨb[�����0yB��	�w�tIWtD7tBW�z��x~�v^�t��$�ď�/�sNć��ǹ��7G�37�2�!�L�L��b��+v��B�t�^����E}+��D!�dUReTbZz�O��'���w���=��ϙ��������K|����ros6_� ��<�}x��;|�B�@����w<�q��m��i<��x�øԡ���r'o�&g�%ߙ�%9�Yca+��S8
3�"#�!�n��~l�:���9	�"<�Dx�qa8~�9���ۘ��8a5N��{0��h|�f\�^l�װ��'@/<�`�B����&���O��N��M����E�h�b�Z�]+�R'UR�h�B�
j��2Q?�S��i]��*Q=�S<�S;�S�ƕ�L_�m�+tLU�Ji��u�P��'u�&��iE+eҳ���d�X�
�%����
�ة�7�S���X�W�"5e��:ZAϊ�e�Uê�U����F�ը*P"�:��-�>@h����,X:h�@
[^ %�d�\��j�C2�d�5[B�܀g�Ϗ�,�`N!�C{�����8��#���:��Np\�flN.�#.�ለH��h!D��3F���4m��YSM�2;��I�[R�[�@�|ҐKd��[���z�P��=�C�e��Y�* �5�5�%��r��\_$��\R 
U�Ca�����W�ѩ~s�ч����dp2�G��_/��0��l���
],�I:���Tyu�8��b���ru��M]����Ցm�~���(�b��T���W���|N>���+�m}Y�]&J>�aK~��e����9��܄��ȷ��;7�>cl�#�u&��p���^!r*E�p��[�Ux��AtG6kYy����W������WP۹����� .A����@��0-$}H�E��ڶ���uz�T�hq�[�*nh��BM`	����Hܥ\JB!p�X>��,�{��w����z����c���8��׽j��W`�W��+�b���9��Ŝ��XqY�Ý�V���h�fr���6�9[��ѫڨUkG��P��ͽNq;�i���y�e63�kH��3$\ʐ1$�|s\N5.�$���ͽ���m�V5��b��e&��|�(�b(f�?q�y��\խ,kXo7w�wjl�&P
�t�X��8�50g�0���K��S�qV"%�-��z���ͪ�0ѹ�U
��O���#�<jթ�AĘ <YeXV��w�*IK@T����LǼ�'�E}Pߣ�b�@uP���P��)�im�[�1R1S!S0�U�:i���\ldDD$�� ɰ!��0��6 }BBS�ep����x��l�}�SVmZf��v[��/�T�����qZFr��Oh��ʏIP]^�A����$*͂���'����'I~�G�yi�Uy_���C��+�ܫ��<+P	9s���0��z��JI�9�]0��0ڄ����;�V�h�#��f�w��j���Q��o�����u��T���FhkyA���{��p4�u��������S�a��R���GvX�poe?�e޸�a��h�!3!��0ԥadX\���]���W��q�Ӧ� ��!��t_��bv ͋?x	O9�����]�s�a�)�%v��r#�E2,��X���:�������@	Uw���1���<К�ѽ)^��m�m<]9��e�����wa�oD+T�VRs8�[�ߵ�����)a��VRSCC       [remap]

importer="font_data_dynamic"
type="FontFile"
uid="uid://erdgllynwqkw"
path="res://.godot/imported/Roboto-Bold.ttf-3674de3d9ad3ee757cd4b4a89f1e126d.fontdata"
          RSRC                    AudioBusLayout            ��������                                                  resource_local_to_scene    resource_name    buffer_length    tap_back_pos 	   fft_size    script    bus/0/name    bus/0/solo    bus/0/mute    bus/0/bypass_fx    bus/0/volume_db    bus/0/send    bus/1/name    bus/1/solo    bus/1/mute    bus/1/bypass_fx    bus/1/volume_db    bus/1/send    bus/1/effect/0/effect    bus/1/effect/0/enabled        *   local://AudioEffectSpectrumAnalyzer_odciy K      ,   res://examples_dd3d/VisualizerAudioBus.tres �         AudioEffectSpectrumAnalyzer             SpectrumAnalyzer                   AudioBusLayout             
        ��   ,      MusicAnalyzer                                            ,      Master                             RSRC            GDSCd   �
  (�/�`�	� ��ZB ���x�~��S�+�����4���9�	@�E�q�%��4��_��`@� ��wS�5K!�H��R; K K �V��V��SYIi��s'mQ���v�U��E�?k�b�6ũm��V��iiKj�Ҷ��<��F~� �4H�":�f�H| �f�לZİL ��;-?��0CǎRI3����^�Rg���8x�8�l�X�mi��i+��U�NEIM|pr����l`
)i9�@��{��޻.���o�?��c�I~
D����'FD���&%%,<q0�1!	?��� � �"ʀ�%�Ym�H���NӲ�!M�W�e��� �N29(_���j��3�\���N����5��fYr;��*�
�\��m�IA*;c�1�\v�&D�����hv��}��������5���/�����D̡{NVx6��u8�6����"��[����蘫u�A[�dLOls
,g0F;W�W,Ǳ=�J����^ FNC�`WZ��].'7G�>���&�Y<.~&o�Ȩ�La�F�	6�gY���K�{	1bHvmϘ�L�'S�]:�������
�$�����>��2�]�����==>�t�L��[�měE��\.�H&y�d�FgR���F���6]�_(�ĳ��X�Gi�<�]�#?�������^�<y�ah�k�Uk">��v�_z���?�'��a����B�i���xА�/bN���b���_�P9'�~z}et{���d�s���Yo1�-�u�xv��R�I`���]���y�Ó��=1�[���~�֥z
�ܲZ\����}��c���7}D+첯0t#�㻒v�����e�M�s���W��R[]�a���t ��e�         RSRC                    PackedScene            ��������                                                  resource_local_to_scene    resource_name    script/source 	   _bundled    script           local://GDScript_wbn2v 8         local://GDScript_7yy7l }         local://PackedScene_a4xml �      	   GDScript          (  extends Node3D


func _ready():
	if FileAccess.file_exists("SUCCESS"):
		DirAccess.remove_absolute("SUCCESS")
	
	print("DD3D Version: ", ProjectSettings.get_setting("debug_draw_3d/settings/updates/addon_version"))
	print("Engine Version: ", Engine.get_version_info())
	print("Distribution: ", OS.get_distribution_name())
	print("OS Name: ", OS.get_name())
	print("OS Version: ", OS.get_version())
	print("CPU: ", OS.get_processor_name())
	print("CPU Count: ", OS.get_processor_count())
	print("CPU Architecture: ", Engine.get_architecture_name())
	
	var res = await $Runner.call(&"start") if $Runner.has_method(&"start") else false
	print("Headless runner returns: ", res)
	
	if res:
		var f = FileAccess.open("SUCCESS", FileAccess.WRITE)
		f.store_8(1)
	
	print()
	get_tree().quit(0 if res == true else 1)
 	   GDScript          Y  extends Node3D


# Some API calls to test library integration
func start() -> bool:
	## wait for call_deferred in DebugDraw init code
	await get_tree().process_frame
	await get_tree().process_frame
	
	DebugDrawManager.debug_enabled = true
	print()
	print("Start of testing.")
	
	if true:
		var _s = DebugDraw3D.new_scoped_config().set_thickness(0.1)
		DebugDraw3D.draw_box(Vector3.ZERO, Quaternion.IDENTITY, Vector3.ONE, DebugDraw3D.empty_color, true, 1.2)
	DebugDraw3D.draw_sphere(Vector3.ZERO, 0.5, DebugDraw3D.empty_color, 1.5)
	DebugDraw3D.draw_gizmo(Transform3D().scaled_local(Vector3.ONE*0.4), Color.BROWN, true, 2.5)
	DebugDraw2D.set_text("FPS", "0")
	DebugDraw2D.begin_text_group("Info")
	DebugDraw2D.set_text("INFO", "hello", 1, DebugDraw2D.empty_color, 1)
	DebugDraw2D.end_text_group()
	
	await get_tree().process_frame
	
	DebugDraw3D.draw_sphere(Vector3.ZERO)
	DebugDraw3D.config.frustum_length_scale = 0.07
	print("frustum_length_scale: ", DebugDraw3D.config.frustum_length_scale)
	
	await get_tree().create_timer(2).timeout
	
	DebugDrawManager.clear_all()
	
	print("End of testing.")
	
	return true
    PackedScene          	         names "         HeadlessTest    script    Node3D    Runner 
   transform 	   Camera3D    current    	   variants                             г]?���F�>䃄��Fw?탄>   ��e��%V?                 �?              �?              �?          �?            node_count             nodes     &   ��������       ����                            ����                           ����                          ����                         conn_count              conns               node_paths              editable_instances              version             RSRC GST2   @   @      ����               @ @        �  RIFF�  WEBPVP8L�  /?��t!���Qжßo�Di�L���D��� �۶��m����LR��v�m�s۶m۶m۶=��is�5]�\�L���m����vֶ:��q�k۶w�N�v9�m۶=k��&.l�׿ms??K�&F����܎����错�2����wTffngXd�8rc�@նMSr��6q�FhD"�&�I�Mj��#��&�����H���č':pI���z��pѳ&   ]�J�    	��" "�D0(� @  ���ѳ( U
 �i�E�$��W�����H�I5�ޡ߉`O���eG�uW��eԊ���mEO
�EA�Xt�H$R$(A�H�H��HD�  ��l0.w��-Z��J ���=F=t-�2�&7\�2	@$E"��"E�tё��Б��Е�L�ԕ���\���b{�]�w7����!#-�e�������@��
�T AD)�#����Jȧ��� �D�)��0��,�v�k���L{����"�I�pL��aW"  �R "@(@�*�H�����4�D�H�a�D�D
�t�ʘ��c2O�����ٛ��4�E �@@դ�}�Б  "" P#"� x���{�wZ?O��F"A$�JeA,�H)ҕ�ȟ\u/��k�ݴ[�zE�p��� !�P@  DD 8��'�t�Dt�o�x��g�M`(A�H *"U
*bѷ(�b�Q�.���y��^d��0�h���]��#@�v�T����!" J�,���O1+u���%�""���P$� �,: lG�_���y��Q_k� hTE� ""@�� `�������wG{$D�^�;~�2P�P�*��J�
�H_�D��B���ncl�r3"ѬKU� QD	D(��E�ֽ,�M���m0>[�|�%�(�!�F�R0%�n��+a3S�-(,@% F;�@)� <�Ї<��;#��x�����?F�aD� �� �(@�Lʵ�;̪�=��0- 2���Y"2lAp��uq�c�Y����{�z�w��  ��@`% �
�Dr�^n�Y��,�C,i��@	���" (����?}���y�7���f��;^���n���~��Q�QD��DD,�D0� �B���D�e�1�8 ��-0�DD�} B<����-� ���{&ϻ��1�8:�^�����=��v|�we���@�"�P�""��Q��M=b��  D0$hY^��S�<�V�x����!` 
P@ph���>p�w�������o���6�w��Q%�hݭ���(P�� T)��F,b	   � (C~��;<yح>���3�_���]���{<b�_��CQ>{�mΪ�H� ��u?�1��� !@�>�f��"��@J�J �Сt @D)h��h��=q�����r���w��s�8O����<�s���ݐy���- qV�GH� ��v'̾m�~�7���E��ږ~Ľ�RR�ls�m??��!��@T���`�% ��_�����;w| �񂱿��%������V?�+���OP " �@{d��UNY��G�6��b���֕{�a����O}����wOi�6מ�<W}Q��� � ��������Uo/z��Z��$d��xܼ��������g���W(�` D(��9���%�q��g�:�{����?��ځ[�����I�������]��+?� @h�B @	@��nwƲ�ht����ּt�Qy��_�=������� � �u���k��y����y�-6T�W�a��M@��[���6�デ9�׺��9Gu�U�@���������������nq_H &O[ ����O��?~��J^��ܓ��"`��#���C�U���e/1�v���w�9"�
��W?��-O��sD��U �� �*U�?�E��iЯ9���(�   ���F��o?z���� e�U""�=������6m:�f��+��4������	D�<g���$��[��-���۱r�(�E�X��?|i���������K� �$��\$��N�?����ٙ8�0c���̌nZw��������{���M��?n۔DN�;��w���8��y��4�;w %PA@	y��?���o���a��Ұ��q:`X��v��K
o�r�]~s�C����@�es*f��V�b×��2�^[�l�  &���`��M''��Y��w|�45� T) �����NY��/��ǹm�{_�� �Žw�  ���Ӟ_-������L�  �,�xۆo��]���s>f��� �vB��`���� ��� �3N �oz�O���n�.��i<z��U�鿭���ݷ�(wE�/<k�%��h��H@ FD��K߶i�E'�}�n T��6��a;�o�mɃfݰfktؼ��:邈 �H$�(A��S�����u�'����t��G�>��k���c+���u.�>&����*��ۯ�8"�^#Ѷ�����m���#����N�W��È��'�'��:�EW %���,� y���y� &�����/���7���9��z�hn�{͕�{����u�U�� D 0������z�G$����=0 89Ӆo�	� ��cՊ���&"��?�C���ڰ駽�@:� �@��N�;5 ���{q�._iwӏy��So���wy{ɫ\��$�������{]��s�B�@�؜:d��#�a ҄U
 ��4�n `��K��9��\� ����!����k��^_�xm�W�4�8 ��|�u�Q49��S��{{��;~��t;����"2�"k>ja�U;N�5#��ɵ�@�#�`D��� DD"f^{��^ (�BC� Qze�����4�g���C� `,��^M<�@����;�/o��G:��+"B��ز�c��u{ln���A[ hª������C$P@0��#�Ԫ�����#��*U�s�-��u����绝��) ����  �����{V	��@DD�w���V?g,}�a��eN|��}\� ��p��z��E���.�Ϯ�� M�o=��^�e/��V@� ��~� D�'~�^Gnxs��@0DDD\��I�k�q+�?iVڸu�1`N��V�r�أ ���z[�L?�ipÒj�fEl9z�� P"U�0� �/��yo��~��{��D@�[�	����7��tJ_<pݫ(��7J�4�Vs���:��O�XvP7cL����+����� 0V{ǆ-�4_����e�Ϛ1~��� $BLS�( �Ϸ<����gw��'�|5"b��g^�5���~��|�o� �@@ `rM.[��^�)]�nZ�P;���U�Z��������6�*��ׯM�l���,/8  ""4&1	 0 P����/�Ҿ�����?��0_~�(	I� �?\����y��ԥ9�՚b  Q0��L��eK��k?���2{u�+���+�Fm�o⽚h�>��n9�(w4M�F&1� @Z�˯~�~��^�U����@ཉ�8yզ��p�'��`  `(�ԙ�մ�sH� Pcr���efn�g�����fD��;� s�/���-G����aV5_3�JA#}�o Q��<uƫ}��o��m?c�/����~��q˛z����=��   %��m藘����P6�w؆Z��c-kz�F���C�;� �!�@�Tc
S @) 
PZ�����t���>������8����T� ��Ј�w<��V���Ż�>a�9,|w�}��m��6^"�[&� �H+̈ F��� �#  7U}p��3n��@D<v���⬪��ַ���活���|��_����J�@��"@ ���G�  ��, 7W} P �x��/u�/��w���§D��L�������D{���D U@ @�Q �R�
  @ E	��Ȑy��.<n��y��'��{� ~s���oX��=K�,sC˹  �a#F����hQ"���ֳ 	�@���gy܆}ί2~r뛉v�
�J\7�������[N�{�:0�@D@��u<�G�*�H���� 
 �� ���^���߹v�eu>��g ���g$c�}�<�.�bM�ˀ��]#  �UeZ  "��D���� w��?g�e��VC( ��u��v��v۫t�ol@k���( B�>00� RB	��䏓�7�q>��D� ��~�'�:��+���x3� CU
 ��=�������{��c>���!� {���㍋P0"@T��r/ "\�(s���~z�U�����G.{�3���Q�M��� �����V�[L��kvۯ�Sl�{ǺW лoM�ŞE���~[ ��D�>� �H��W��ϫ~�l�q�  ��Ԣ��>g�%�)Dh���Nǅ��^w˫w[��^����%�2�y̆��۵�IPD����f<r��]Q�K�~�k�|	q�����'�z�x�so���ى�6 ����Us�=�>��Z>����?_?o��cv��>Vw��@�(�`  **F�]""�?�����(2���y������.9?��ڵ@����L����g$c�}:?��?/iR7)S�/8��	@Sh��(Gy����^�m��7E�x��=~�r��Y� ��JwD
  %@ ����V~��VF� ��/�(�6���@��)B��tЈ�'��Ǟ|��yӾĦsǧ�=�[�Mÿ�ʋ���o��VZ�Dİ�A��w�c������%�?����Cv+�ou�{l-:��>'�� F�i�*U߻��'��G�{��g�c�>�C6�ϬK.��~ ���,�cy��    Q0�� �u��z�U���eb�:�wػd����g���vz���N (U_U t�Xb�Ac!�l]s]�U�z�7����;j�yd��,�h������7�_ 
� H %���`hEV�����V#�Z��n#g�A�M��29m��onX��>ͱkg"�h��)(�	���|�p�����w�n�Mg�Y���_��+�:;��s�m>� �  (JD�+U�`�e""Jƪ^�~�{>Ԭ���VL%o褭�4߯Y�CT4��	�����"�'[e����x��W:g���g���@  J��`(FDD�7OM� ��of�!���b@n�V @ڪv�1و?�z	0D�j�� JD�P"�*bDD0Ўg�jG D���  @�eݮ�Z�[C ��e@����n ( "0le � �hT��S  ���b�c�J�+`�h�";�E�D�*d��峘Q ��G�.®��6	�� @D@��i]WBi�C�"�4R�K(�4�x�kܰ�|��cB�
 �R�H4 % @ ��F@ D��*Ug�*$ ��4Z�᪨n� K�:( B� @ DHk�Ѐ ���@ T%t�z+&)U
Z�a�L�	@H� �( ��QH�`D�U @�� LV�*#U�E Zh\ [remap]

importer="texture"
type="CompressedTexture2D"
uid="uid://deelh6rbgusgd"
path="res://.godot/imported/icon.png-487276ed1e3a0c39cad0279d744ee560.ctex"
metadata={
"vram_texture": false
}
                [remap]

path="res://examples_dd3d/addon_icon.gdc"
             [remap]

path="res://.godot/exported/133200997/export-e7de4a5fa3dad1fa22f4b34ade96ef28-addon_icon.scn"
         [remap]

path="res://examples_dd3d/DebugDrawDemoScene.gdc"
     [remap]

path="res://.godot/exported/133200997/export-3f6dfedcd5ab4046d211567bf6df0470-DebugDrawDemoScene.scn"
 [remap]

path="res://examples_dd3d/demo_camera_movement.gdc"
   [remap]

path="res://examples_dd3d/demo_music_visualizer.gdc"
  [remap]

path="res://examples_dd3d/demo_settings_panel.gdc"
    [remap]

path="res://examples_dd3d/demo_web_docs_version_select.gdc"
           [remap]

path="res://.godot/exported/133200997/export-9aea7a5946d3b0c2d9d1e1b0c1a72c60-VisualizerAudioBus.res"
 [remap]

path="res://check-only_test.gdc"
      [remap]

path="res://.godot/exported/133200997/export-9c301d5b8a21b7b00cee58224ccb0626-headless_test.scn"
      list=[]
        �PNG

   IHDR   @   @   �iq�   6eXIfMM *      H      H                      �x�   	oFFs         �*��   	pHYs    ��~�   CtEXtFile C:\My\Projects\GE\Addons\godot_debug_draw_3d\images\icon.blendCn4   tEXtDate 2023/11/15 07:58:287�M8   tEXtTime 00:00:00:01��&�   	tEXtFrame 001�Q"   tEXtCamera Camera.001�Y   tEXtScene Scene�!]�   tEXtRenderTime 00:00.08���s  �IDATx}[��յ�O�����
�Q)v1�ш��~y�أy��yF#I�3��i4j�41�(��a* �%" e�e�]���y��w�E���o�s�-ߝ� �+�@�)
Ŷ5�?�o��z�%�@�f2Jy�m4�i8�����lJ�.�%Cf��������f��A�t��Z���Ħ�!�q��>��P	ƌ#��K�!v��)�����,t˄�7�vmZO^+����S�hi��x|���P��H$4:��<�A�*��X���2D"a�`0��A���H�m:��ZH��$�[M[ȣx���i����~>�w���6|���hm�ה�foޱc��<k+&�at� #$ i8R�X$�O��pX��˧	+
r��Y:�B�=�3(,��Ƈ:B!}\�#��4m`[a�#��X,���
TWW�����Y��]��������;6�/���φ9a�p���F�q:��E�-H:J�n�ז�9�}8e3�����>��,���{"E��N��|Zk��'n��1��ը��q�hh�ǰaC�wo+���'���y��������� �H�?[���s4� 2�_���(�9Mjuނ�}?/���Hc��9�8�����\.�`�lε%ő�d�'���~tww����խ��=zf̘�m۶�/"{����P��P�ix՜��%�1��:�
� �-d������.�נ:/��YTVd��2GK:}�ց=���y�Aa`��:��@tuu���MM�]&̜97~��P(��U� ��4+P_-����|\0Mw5AG�JX�H��>�x��j�g ���Ν1�x��}�����(S��akS/�Bk��@d29��0+�ɤdA���tSॗ����K�F�5{ ��bI����ITʑ�WES�ISj�e�����l
��;�����:���2X��7لi&�S��Ng<��J6�\ :::������㢋�O5j�U�7ꆈ�2�C��4�`N��qL}k� �"
7��g}���f�kg�w� >�/���OZl��L(�KĂJ��m?[�.3��M�>�W��RKˮ�I���$�?DWV:���m�N��VS���()���Sy��޸WM�/�q���,���>�	s�i�m���0s������`&�����;��9w�)�#'̢mA�`�<ב-�Jq���E��G��1X)��L�ĳ�5pTfxb��0S*[u�:@��V��iN����N��@�[�)&N����J��2 �t�#�e`(�ʁ��\'���:�w�q)|���^�������9}n��(+��s �HM���Vk��c��+V��T"�8��Ⱥ�P-"d�c�)��
־r����g[���Y+��F�Ƣ8���p\;��4�����J�n��ϻ�N���(ӂ��23���\�U��E{(�}��;;����.'�A�PD"���x<�>FH�	� ���Z ��Q���վ��� �{[Z����)�wD�JB�gC�d0�>��Ņ>�g~�U��(�I��k�K�ö�ۧ��@ϟ�/��oIxˮϡ�MF��<篩B�y3�����&����)�G\��i�a�zea
�����1�v��<pi��o��c��5�cS��9�~��j�M�T���@��8�Btv��")�/�;�$Pe����E����H����B�Zd*�~i:ʾ�F�a6m�������c�����J�g�V�C��@���l�����Ay<��E�N:N�j��mʐ>�N�����&���-j���WB��S�V�q�3$~��]�)
畡g��Hl؁�5M���*����$�f�'Ϻ���uӡo�0oL�F$Q^�B�'�ot��Њޞr�����4Sd�s�[@�~J�L�=|�%�m�,D��D�qH��#okuF��ذ.�����X��g�@���(;}��+:\�fP����a}�����Q���cn���vy����D�8^&�U�8gL��-�jq�����L`�5t���͑�SɌ&1��_zr�kPv�rq~��:a��E��#~·���	]�NF��*�Aj�9�:4:6���y�Nׅچ0�|�_~k("�2���g���i�����gq�SC��BMU������8��ț� T��ڌX����[vb6!:�C�iMC=c�(Kc�	�n݋��@��Ñz~Z��7HNZ-���ar�����ݲEF���r��+�F.Q���{��z�ڣXШ���[
��T�aє�	w��^�/k�S�W)���^4	�c'"��-���<G�]@2�Ŝ����SW в=��pL�T�ĳ�Q�1�,W�ȟ1��}qt��7�
9�?\@og���sM	��t+<eܵ��t���<W�ä׶֞���tnR9y����cƠ�ƿ�E1nW0X�
#�µOet(r�f~@��Ǣ��e�O���F�@���9#:Ѻ/���4�G�Ш��lxc�r~xl�n����>�xt����DY�B��ml�kn�׭���6�X\ � *W��QT>��?��ȥ��憢c���J*��++�G9|�4�H^2	�m@����8�<�%����KvT`z6�k�ŜJ��sU?^����"����\]��h�<�1.׏8�1�׶�no:��+YU�
�d q�J��)��t'\G���H���z��T���E�UG"��:����5,%5����9�v�0M�z�U'y���8��K�8JCY����e�Xs�F����[m���6H1�=pt�S�|{³�#�d&booE�V����r�*L� ��p9-�P�_��E��%1u?�D8��d�kSX��'����&���6�b97�gQ[�`�Q���\���Y�Y�<e� /~�~�@g���r^ļ�~`e�+�9����p]5�o���6$nB��뫑��
}/�@�]�^��:o��]����N�σi]+��}��(!B%-��$D�O�\��l6��1}�4|����(��;�K+����f���@�h�Ј;��ŉu��uH/>���U<��a�^<
�x7��/[��SO��ʥā
�0���'I�ϓ��f��T�8���5��k���
�@��4<��Jl��aL�PG�˗M�'��"H�N�6-��Ba9R���ν�8^�x��9��Ղ��ݰ��:�w�!Hܾ��ۣ;e��,�a��G'x_F��8�\��&w��Ox��  �����k[/����1=����x3�2��5,�U��8��$ֵ�`(=u�(�2&ԑD$�eX��;驇 �л,�FJb�*d����������.��z��i�-��|oW�Pe�Z�3�(��;��B���3�`�ٜ�d����',�eY�0?XC��]1\^�Ư���x��l�B� 
�R�[ӯ���Zį�C��<�Y¿ރhk�8?���H2�+���cP8R�Z@��h,���4����E��'�#ks�L�0'Fz߶����:�cI�t�br�2�Έ�n�8�N���鱸��1e
H�g7 ��D���. �{��+Qeb\�0�jK>�y���omF߂	��l8hf�@#Jy,8�`d}?�m�b�\��f�,98�=u[�j�
8�,/�P�Vg�]j�������AwQ�%{�qcQ��7XaFp4�/�	k M�9+��s?qS" �
�,C��C�ʖ@
�7�U,,ovBH���~2�w|��Q@���M���+~(R��@�/䈩öx�yc���\�� 2|[�됺l$�+�(�u�!��$d��"dR��f���@�^�<uI��Fp`~�A-�?�>��R.�n�u�_�CҬ�,nw�6E2��lN8���y�Ѝ�D͒M��,�(#�+���ECHL���ϝ�>�������ѤS�Z�D�"��muRM��U��/��:��Է�)D^%�Hq���@>�]*/3w�_��I���q���Q���*R��c�"uIk�?X�w��,�=]a\�C�\�E���[!Yt4���P�z5���?F�&���C������fQ�_�@�k{q=o:G���Ҿg�CiP��r��{m9����J\��"S����>���/�#��K^����;o� �|���v�Kd����S��s�ܨD�KG!��V�T'H$��AH�4��N$nY����1��Y,�v�
�-� �(덴d��2\;�WڂK��J��)2�yÛ�_�d���v�ٗ�xs�s���t��ٗ$�[�S@���bLl�a���x�$���ȤJ����8E2�����_�	����Q���� �+�p�g8���x;]�K�w�R,M����"0�:�#��$v�����_~
ù�Gq��Ɩ⒤v��� �`` ���<n�/E�;ˏ��3���p���w4"3���!s�P� WW�����`)-��R��.Byc�g_W��r��hK�l ��/�K���X#�G��:��kF���M�6�7oK���ZS�+MMk��c��y�7ˉ��QH����^��Z|��C:+�"��Ȍo��{_E^F[|�޾{zڱ�pL(�t�U����C��;�)��b�L����wD�0f@�}aJ���*�Z��.f�F�ƪpc�"�sp\Y����Z��+�bޭr�l4/=O�쌡rKy挘�w�|�u� �/���p�{I:��<��a��t`��q�쳦d{{WM��ʋ�n�m@q
�\UH&+�3��@�
'�N/8�7c���ƍ����Fn�ߊ2|y�(���ըr͘��?�0(Y�*:�p�&\��1{n��YO�0[�lO���]S<
�Pc�i����֤s6�y��PJ��U����*w)O�Y|��G�I��ٴ
�=mx�F����?��h�4�kt�_Z�$��5zk �ꏨ:��ꠙ��a��P�Ѳ��x-F^}-:>X�=w\�|��R!^<	B�Fp���X�քW���$j�9@ݾ�$P�}���A��G\�h����0"2���L�:' IE3�0-l�� j.�5��ǎ��Y�H���&o�\�����{[0jK<x6f�w�[͟8�ڤv�}�����9��1n� $i����Y4j�F*�ZM�X���g\�m�{�8�~���c0M��z�07%���-�����ȣ=iX��S�����I����� /a�(L�t� +f�ET�~"���SnA#\��+�iߔ���7nA�"���I�eZ�T��cMY��ڄ_ٶ��n�����ؠ��ҙ��G�d��G��3���!� ب��&�Ir�k���z1�	�M���Ε�� C�;�=���n�J��O'��P�}9�8�͞;�ZqŖ��`��x�ab	��^)��}�h��$��3:h�+��'26���7P���幧�z�����^�T1;~�#�%�� �h��C�3�k3j T�@�6�=��������v��	��bMk*�v�E>�d6���,0]�[ �Ak#b�LF��Ï�~�q�r�:8�s������k��`����\�W$*6�k���ņ����n���.�8z6��U=�ݽO4���Cs�S���>����@ȅ��r�Ԡf�DAtz�=;#��s���n@�I'���^�H�DUj�+X�� N:�emG"�s ;�3>�K4��ףgb{�^����t7.�П˫,'��� �2�/D���[;��QV&w�RJr ���pR����9�T���R���s�a��(�cME|.Z�:�E��RHG����W���iѮbx�\�{:���p�!�TW�I΀{y���f�f%Y�Ь�x�%�!U3g�k�M�D���G�}��s�&���?a�EwS��*s>��3�X��Y ����Q�C��'����uc��ڃQ)�u�&���bѰ)����fL��84�����2����*m�i���;�=tt��O���Y'�g�?P7�,�/}�w��h�AGb'|u�|I^���'�Q4�:R$w�6�T��i`4 �.���p��5���n�/��\@}y��2%�^���u�i�++*O-&?����w�Cn�b�����}5س�MR�Q�@���駨8�0�.���O�r.��Bb����/1�[�ھ�M��o~���*�Q�A�oxR+G�8��p��S���aF��U��&��Y���mX��v<�4L�S8�f8����'G�q����!��L���-�4.����J�%e'��z&�ǎ�*�t{�}� �m��+����9
����7���N��U���i���I:K���Qx�i�F6��������iV$�����D�dHQ�N��"8Q�ޔ`0p|�6(�\*��L٢G�h"1xtee�ֺ�*�!��<��_~�u#�cƢ�z�׿�܎��`����7�V�N|�R��Q�<��@j�Q�e��s zr�,��BW>������/�X�!�D5)����� �:�����3���,�������P����2y\��G�͡����}��2~�W�@���Z�Pni��s��d�Wg2-�L���!��]�L�$�镕�b�����	Rځs�0:b�ڦ��VS�9F����8R����y����n����o��<Of��7��� (�
���^��������������W\J�p�n�J�ןJG��<�������]m���Iv�9|t����e��P�˒���*H##��r&���Oέ�����JY��qh����3;�Q-U��A�63��V��󵴨N��.�P��t�s��sŗ]�z9���Γ�|��~4�G��O��{�?��VV&�H��?�R��y)wl&��2pE/ř���5��Z0�Q�K� (L�G��}}�ޟ�g��/)Z���	R�@��0	�h��u�:_�����$�)��Ң*^�G*��c"�@#�1�}�ҘN���t�V�h���>@^�M�>gI�����ߌ�[d�4T�*U��:
"#�x[0)Ōg[~S,8��b�Z$``<y��H�\�*?�,t�A)�3��j�I:P��b��l���   6eXIfMM *      H      H                      �x�    IEND�B`�   }����%4   res://addons/debug_draw_3d/debug_draw_3d.gdextension�"�Cؽ;!   res://examples_dd3d/addon_icon.gd�L�x��#   res://examples_dd3d/addon_icon.tscnf�s�!)   res://examples_dd3d/DebugDrawDemoScene.gd���:^+   res://examples_dd3d/DebugDrawDemoScene.tscn�c[W�l5-   res://examples_dd3d/DebugDrawDemoSceneCS.tscn�qܡܯ>+   res://examples_dd3d/demo_camera_movement.gdE�3;J%,   res://examples_dd3d/demo_music_visualizer.gd�E{���� *   res://examples_dd3d/demo_settings_panel.gd�z����p3   res://examples_dd3d/demo_web_docs_version_select.gdi
�� )   res://examples_dd3d/PixelatedElegance.ttfj	-��b#   res://examples_dd3d/Roboto-Bold.ttf���X��+   res://examples_dd3d/VisualizerAudioBus.tres�����q]   res://check-only_test.gd(L��7��O   res://headless_test.tscn����`g   res://icon.png          res://addons/debug_draw_3d/debug_draw_3d.gdextension
           ECFG      _custom_features         forced_dd3d    application/config/name(         Debug Drawing Utility for Godot    application/run/main_scene4      +   res://examples_dd3d/DebugDrawDemoScene.tscn    application/config/features(   "         4.4    GL Compatibility       application/config/icon         res://icon.png  $   application/config/no_csharp_support            audio/buses/default_bus_layout4      +   res://examples_dd3d/VisualizerAudioBus.tres '   audio/general/default_playback_type.web             audio/driver/output_latency      2      dotnet/project/assembly_name(         Debug Drawing Utility for Godot    gui/timers/tooltip_delay_sec     �������?'   physics/common/physics_ticks_per_second         *   physics/common/max_physics_steps_per_frame         #   rendering/renderer/rendering_method         gl_compatibility*   rendering/renderer/rendering_method.mobile         gl_compatibility4   rendering/textures/vram_compression/import_etc2_astc                        