VLPRDemo
========
车牌识别Demo -  套牌车识别

[2014.2.21 13:22:00]
修正了图片识别模式下，会崩溃的问题，是由于使用了视频识别模式中的全局imageWidth和imageHeight，现在将队列修改成了结构体，包含图像和宽高，目前运行稳定




Revision: 7
Author: karl.li
Date: 2014年2月25日 18:01:47
Message:
完善安全退出线程
----
Modified : /VLPRClonedDemo/LPR_info.h
Added : /VLPRClonedDemo/LoadingDlg.cpp
Added : /VLPRClonedDemo/LoadingDlg.h
Modified : /VLPRClonedDemo/VLPRClonedDemo.aps
Modified : /VLPRClonedDemo/VLPRClonedDemo.rc
Modified : /VLPRClonedDemo/VLPRClonedDemo.vcproj
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.cpp
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.h
Modified : /VLPRClonedDemo/resource.h

Revision: 6
Author: karl.li
Date: 2014年2月25日 16:33:54
Message:
套牌车识别完成
下一步：
1. 创建图片名称格式化工具，更加图片的创建日期来格式化
----
Modified : /VLPRClonedDemo/FileUtil.cpp
Modified : /VLPRClonedDemo/LPRDB.cpp
Modified : /VLPRClonedDemo/LPR_info.h
Modified : /VLPRClonedDemo/VLPRClonedDemo.aps
Modified : /VLPRClonedDemo/VLPRClonedDemo.rc
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.cpp
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.h
Modified : /VLPRClonedDemo/resource.h

Revision: 5
Author: karl.li
Date: 2014年2月25日 13:05:20
Message:
成功获取套牌车，通过数据库对比
下一步：
1. 点击list显示套牌车图片2张
2. 创建图片名称格式化工具，更加图片的创建日期来格式化
----
Modified : /VLPRClonedDemo/FileUtil.cpp
Modified : /VLPRClonedDemo/FileUtil.h
Added : /VLPRClonedDemo/LPRDB.cpp
Added : /VLPRClonedDemo/LPRDB.h
Added : /VLPRClonedDemo/LPR_info.h
Modified : /VLPRClonedDemo/VLPRClonedDemo.aps
Modified : /VLPRClonedDemo/VLPRClonedDemo.rc
Modified : /VLPRClonedDemo/VLPRClonedDemo.vcproj
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.cpp
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.h
Added : /VLPRClonedDemo/VideoUtil.cpp
Added : /VLPRClonedDemo/VideoUtil.h
Modified : /VLPRClonedDemo/public.cpp
Modified : /VLPRClonedDemo/public.h
Modified : /VLPRClonedDemo/resource.h

Revision: 4
Author: karl.li
Date: 2014年2月21日 19:02:21
Message:
增加了SQLITE3
----
Modified : /VLPRClonedDemo/VLPRClonedDemo.aps
Modified : /VLPRClonedDemo/VLPRClonedDemo.cpp
Modified : /VLPRClonedDemo/VLPRClonedDemo.h
Modified : /VLPRClonedDemo/VLPRClonedDemo.rc
Modified : /VLPRClonedDemo/VLPRClonedDemo.vcproj
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.cpp
Modified : /VLPRClonedDemo/VLPRClonedDemoDlg.h
Added : /VLPRClonedDemo/rabbitmq
Added : /VLPRClonedDemo/sqlite
Added : /VLPRClonedDemo/sqlite/sqlite3.c
Added : /VLPRClonedDemo/sqlite/sqlite3.h
Added : /VLPRClonedDemo/sqlite/sqlite3ext.h

Revision: 3
Author: karl.li
Date: 2014年2月21日 14:34:58
Message:

----
Modified : /.gitignore

Revision: 2
Author: karl.li
Date: 2014年2月21日 14:23:06
Message:

----
Added : /.gitattributes
Added : /.gitignore

Revision: 1
Author: karl.li
Date: 2014年2月21日 13:26:28
Message:
车牌识别Demo -  套牌车识别,,修正了图片识别模式下，会崩溃的问题，是由于使用了视频识别模式中的全局imageWidth和imageHeight，现在将队列修改成了结构体，包含图像和宽高，目前运行稳定


----
Added : /README.md
Added : /Release
Added : /Release/RecoCore.dll
Added : /Release/TH_PLATEID.dll
Added : /VLPRClonedDemo
Added : /VLPRClonedDemo/FileUtil.cpp
Added : /VLPRClonedDemo/FileUtil.h
Added : /VLPRClonedDemo/ReadMe.txt
Added : /VLPRClonedDemo/VLPRClonedDemo.aps
Added : /VLPRClonedDemo/VLPRClonedDemo.cpp
Added : /VLPRClonedDemo/VLPRClonedDemo.h
Added : /VLPRClonedDemo/VLPRClonedDemo.rc
Added : /VLPRClonedDemo/VLPRClonedDemo.vcproj
Added : /VLPRClonedDemo/VLPRClonedDemoDlg.cpp
Added : /VLPRClonedDemo/VLPRClonedDemoDlg.h
Added : /VLPRClonedDemo/public.cpp
Added : /VLPRClonedDemo/public.h
Added : /VLPRClonedDemo/res
Added : /VLPRClonedDemo/res/VLPRClonedDemo.ico
Added : /VLPRClonedDemo/res/VLPRClonedDemo.rc2
Added : /VLPRClonedDemo/resource.h
Added : /VLPRClonedDemo/stdafx.cpp
Added : /VLPRClonedDemo/stdafx.h
Added : /VLPRClonedDemo/targetver.h
Added : /VLPRClonedDemo.sln
Added : /include
Added : /include/SkinPPWTL.h
Added : /include/TFLPRecognition.h
Added : /include/TH_ErrorDef.h
Added : /include/TH_PlateID.h
Added : /lib
Added : /lib/SkinPPWTL.lib
Added : /lib/TH_PLATEID.lib



