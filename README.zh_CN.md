# DDE Launchpad

深度桌面环境的“启动器”或者说“开始菜单”性质组件。

## 依赖

查看 `debian/control` 文件来了解此项目的构建与运行时依赖，或者使用 `cmake` 检查缺失的必要组件。

## 构建

常规的 CMake 构建步骤，即：

```shell
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

构建完毕后，将可在构建目录内获得名为 `dde-launchpad` 的二进制文件。此时你也可以通过如下方式进行安装：

```shell
$ cmake --build . --target install # 当你知道这条命令的作用时再执行它
```

为在 *deepin* 桌面发行版进行此软件包的构建，我们还提供了一个 `debian` 目录。若要构建软件包，可参照下面的命令进行构建：

```shell
$ sudo apt build-dep . # 安装构建依赖
$ dpkg-buildpackage -uc -us -nc -b # 构建二进制软件包
```

## 许可协议

**dde-launchpad** 使用 [GPL-3.0-or-later](LICENSE) 许可协议进行发布。