# Alembic for Unity

**Latest package: [AlembicForUnity.unitypackage](https://github.com/unity3d-jp/AlembicImporter/releases/download/20180413/AlembicForUnity.unitypackage)**
**不要克隆这个库，除非你想从源代码修改这个插件.**

[English](https://translate.google.com/translate?sl=ja&tl=en&u=https://github.com/unity3d-jp/AlembicImporter) (by Google Translate)
- [Alembic?](#alembic)
- [Alembic Importer](#alembic-importer)
- [Alembic Exporter](#alembic-exporter)

## Alembic?
Alembic 主要用于影像业界的数据格式，用于存储巨大的顶点缓存数据。使用Alembic将影像、Skining和动力学等模拟结果转化为顶点缓存。
Alembic 本家: http://www.alembic.io/

Alembic的导入和导出，影像资料等各种模拟结果传递给DCC工具，支持dcc工具。Unity视频导出导入都可以使用DCC工具,。也适用于游戏的3d录像。
本插件在Unity实现Alembic的导出和导出使用。


Windows (32bit & 64bit)、Mac、Linux  Unity 2017.1 版本确认
要想使用它，首先要把这个包import到项目中。。
[AlembicForUnity.unitypackage](https://github.com/unity3d-jp/AlembicImporter/releases/download/20180413/AlembicForUnity.unitypackage)

## Alembic Importer
![example](Screenshots/alembic_example.gif)

Alembic 将文件中包含的节点在Unity作为GameObject进行加载，包含PolyMesh的节点也生成MeshFilter和MeshRenderer，从文件中流传送数据进行加载。现在支持Camera、PolyMesh、Points的播放。

把包装植入项目后、Assets -> Import New Asset 找到 .abc 指定文件的话，会生成对应的prefab。
Project 在窗口 prefab 通过选择可以变更端口设定。

![import settings](https://user-images.githubusercontent.com/1488611/35656675-f27445c2-073b-11e8-8365-4060cbff4896.png)

- "Normals" .abc 设定是使用文件的法线，还是根据顶点位置计算。默认的
 "Compute If Missing"  .abc 文件有法线的话就使用，没有的话就计算，大部分情况下这样应该没问题。

 - "Tangents" 是计算切线的设定。因为abc文件没有切线，所以是计算还是不计算的两种选择。但是，切线的计算需要法线和UV，如果欠缺这些，“Tangents”和“Compute”也不能进行计算。
 虽然默认是有效的，但是切线的计算是很重的处理，不需要的情况下无效的话，就可以预见再生的高速化。

 - "Camera Aspect Ratio" 设置相机的纵横比。是使用.abc文件的相机参数，还是使用Unity侧画面的纵横比。

 - "Swap Handedness", "Swap Face Winding", "Turn Quad Edges" 是将X方向反转，还是将多边形的方向反转，或者是将四边形分割成三角形时，将三角形的排列反转。

 - "Interpolate Samples" 是进行动画片的内插的设定。如果这是有效的，Transform、相机和拓扑不变化(=顶点数和索引不变)的Mesh就会得到动画的内插。
 - 如果有效，或者如果abc文件包含velocity数据，可以将velocity数据传递给整形器。
包装上附带的 Alembic/Standard 是正常的 Standard 上面 velocity  motion vector 添加了生成的着色器。在需要motion vector的情况下会有帮助，比如后期效果的MotionBlur。
- 如果你想在自己的整形器中添加motion vector生成功能，在SubShader中
UsePass "Hidden/Alembic/MotionVectors/MOTIONVECTORS"
这一行加起来应该就能对应了。内部的想知道详细情况,请参照alembicmotionvectors.cginc。(因为向第4个UV传递velocity数据，以此为基础计算出1帧前的顶点位置)

左边是未加工的，右边是输出motion vector [Post Processing Stack](https://github.com/Unity-Technologies/PostProcessing) 的MotionBlur状态

![velocity](https://user-images.githubusercontent.com/1488611/35801196-c587e75a-0aae-11e8-8eda-da4eae575831.png)

- Maya的shading group可以作为submesh进入。

需要在Maya侧启用“Write Face Sets”选项来进行交换。请注意这个选项默认为关闭。

不限于Maya，如果是与Face Set的漏洞利用对应的工具，素材的设定应该可以作为submesh导入。

也支持Maya的vertex color和additional UV set。

需要分别在Maya一侧启用“Write Color Sets”，“Write UV Sets”选项来进行交换。这些默认也是关闭的。

-因为这些是Maya的扩展，所以其他工具不支持的可能性很高。

Maya的Alembic漏洞利用设定

![](https://user-images.githubusercontent.com/1488611/35655697-86d367e4-0736-11e8-9d28-0b3cb37fd5f0.png)


- 由植入产生的prefab有一个叫做AlembicStreamPlayer的组件，它负责播放。移动Time参数可以确认Mesh等的移动。控制Timeline和脚本播放动画。
Vertex Motion Scale是计算velocity时的倍率。越大的velocity越大，在后效果MotionBlur中会出现激烈的模糊。

-为Timeline准备了名为Alembic Track的专用卡车。(Add -> UTJ.Alembic -> Alembic Track追加轨迹，Add Alembic Shot Asset Clip追加剪辑，设定Alembic对象)

![](https://user-images.githubusercontent.com/1488611/35694278-2d6026be-07c4-11e8-98b6-c0b72ff10708.gif)

- .abc ファイルは Assets/StreamingAssets 请注意以下会制作复印件。这是因为为了从文件中流传送数据，即使是build后也需要保留abc文件。

## Alembic Exporter
![example](Screenshots/AlembicExporter.gif)

- 把Unity场景中的地理位置写在Alembic上。

MeshRenderer, SkinnedMeshRenderer, ParticleSystem(输出为point cache)，支持Camera的开头，如果写自定义处理程序，也能输出独自的数据。

为了进行交换，将AlembicExporter组件添加到适当的对象中。

需要注意的是，**AlembicExporter组件在添加时自动使Batching失效。**如果一直有效的话，会写出被Batching后的Mesh组，根据情况数据会膨胀数倍，而且结果也会改变。

如果想改变Batching的设定，设定项目在Edit -> Project Settings -> Player的Rendering项目中。

以下是AlembicExporter的各项目的说明。

<img align="right" src="Screenshots/AlembicExporter.png">

- Output Path
指定输出路径。
- Archive Type
Alembic的格式的指定。一般认为保持Ogawa没有问题。
- Time Sampling Type
指定捕获的间隔。
Unity的情况下，Alembic一侧的帧间间隔总是恒定的(1 / Frame Rate秒)。然后,uniform fix delta time有效呢,而开始了的情况下,如果* * time.maxdeltatime改写unity方面也三角洲时间固定* *。影像制作的情况下，这应该是可取的行为，但是如果是独自管理Time.maxDeltaTime的话，就需要注意了。
在Acyclic的情况下，Unity侧的delta时间就那样变成Alembic侧的帧间间隔。当然间隔不是一定的，但是对游戏进行的影响是最小的。主要是设想游戏的3d录像的模式。
start time alembic方面开始的时间。frame如果速率是time sampling type uniform alembic方面的帧之间的耐力。
- Xform Type
物体的位置,旋转,比例尺为个别记录(trs)是否队伍中记录的(matrix)的选择。TRS应该没什么问题。
- Swap Handedness
有效的左手和右手坐标系/夹放入坐标系转换处理。
很多DCC工具都是与Unity相反的坐标系，所以大部分都是有效的。
- swap faces
反转面的正反面。
- scale
单位转换为假设的比例尺值,例如0.1缩短尺寸转换输出。位置和速度的作用。
-包括
Entire Scene的情况正如字面一样，捕捉场景内可捕捉的全部对象。
current branch其alembic exporter组件有gameobject则截取只以下的圣诞树。
- ignore disabled
这是有效的,如果被disabled物体则截取对象组件也被除外。
- Capture(组件名称)
各指定组件的问题的有效/无效。
- begin / end capture, one shot
开始/停止呢。one shot目前仅1帧则截取。这些从脚本begincapture () / endcapture () / oneshot(称呼)可以访问该功能。

现状俘获对象对象在俘获开始时被决定，中途不增减。因此,物体的enabled / disabled则截取的中途改变也影响不了,则截取开始后被生成的物体不会被呢。

需要注意在捕获过程中删除对象。如果那个物体的问题不能被中断,其结果的样品数驳杂alembic文件有些不规则的状态,因此,不能正确处理软件也说不定。最好的避免场景吧。

Alembic侧的节点名字被赋予象“(0000283C)”一样的ID。这是名字的避免冲突的措施。(alembic是一个阶层,名字相同节点不能有多个规则。)

另外,视频是目前未完全的支持。
