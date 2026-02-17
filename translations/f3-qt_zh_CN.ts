<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>HelpWindow</name>
    <message>
        <location filename="../helpwindow.ui" line="14"/>
        <location filename="../helpwindow.ui" line="120"/>
        <source>Help</source>
        <translation>帮助</translation>
    </message>
    <message>
        <location filename="../helpwindow.ui" line="61"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:20pt;&quot;&gt;Fight Flash Fraud&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:20pt;&quot;&gt;Fight Flash Fraud&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../helpwindow.ui" line="78"/>
        <source>About</source>
        <translation>关于</translation>
    </message>
    <message>
        <location filename="../helpwindow.ui" line="93"/>
        <source>Author</source>
        <translation>作者</translation>
    </message>
    <message>
        <location filename="../helpwindow.cpp" line="11"/>
        <source>Ver: %1</source>
        <translation>版本：%1</translation>
    </message>
    <message>
        <location filename="../helpwindow.cpp" line="12"/>
        <source>This program is a free software.

You can redistribute it and/or modify it under the terms of the GNU Library General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

This package is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public License for more details.
</source>
        <translation>本程序是自由软件。

你可以按照自由软件基金会出版的GNU通用公共许可证（GPL）条款来修改和重新发布本程序；GPL许可证的版本可以是第三版，或者（根据你的选择）更高的版本。

发布这一程序的目的是希望它能有用，但作者对此不做任何保证。作者也不保证本程序的经济价值，或特定场景下的适用性。请参见GNU通用公共许可证（GPL）的文本以便详细了解相关条款。
</translation>
    </message>
    <message>
        <location filename="../helpwindow.cpp" line="21"/>
        <source>&lt;p&gt;GUI Author: Tianze Wang &amp;lt;&lt;a href=&quot;mailto:zwpwjwtz@126.com&quot;&gt;zwpwjwtz@126.com&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;p&gt;Author of F3: Michel Machado &amp;lt;&lt;a href=&quot;mailto:michel@digirati.com.br&quot;&gt;michel@digirati.com.br&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;p&gt;Project Home: &lt;a href=&quot;https://github.com/zwpwjwtz/f3-qt&quot;&gt;Github&lt;/a&gt;&lt;p align=&quot;center&quot;&gt;Feel free to report bugs and give suggestions!&lt;/p&gt;</source>
        <translation>&lt;p&gt;图形化界面的作者：Tianze Wang &amp;lt;&lt;a href=&quot;mailto:zwpwjwtz@126.com&quot;&gt;zwpwjwtz@126.com&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;p&gt;F3的作者: Michel Machado &amp;lt;&lt;a href=&quot;mailto:michel@digirati.com.br&quot;&gt;michel@digirati.com.br&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;p&gt;项目主页：&lt;a href=&quot;https://github.com/zwpwjwtz/f3-qt&quot;&gt;Github&lt;/a&gt;&lt;p align=&quot;center&quot;&gt;欢迎报告软件问题和提供建议！&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../helpwindow.cpp" line="25"/>
        <source>&lt;h2&gt;Operation mode&lt;/h2&gt;&lt;h3&gt;Basic Mode:&lt;/h3&gt;&lt;p&gt;Fill the freespace on the mounted card using f3write and read back the contents with f3read to check for corruption and determine the actual size of the card.&lt;/p&gt;&lt;h3&gt;Advanced Mode (requires root):&lt;/h3&gt;&lt;ul&gt;&lt;li&gt;Quick Test - Uses f3probe to quickly check a card by reading/writing directly to blocks. When this option is unchecked, the card will be mounted and f3write/f3read will be used to verify card.&lt;/li&gt;&lt;li&gt;Use less memory - Slower test with block restoration but requires less memory. &lt;p&gt;&lt;b&gt;Note&lt;/b&gt; in some cases (i.e. power is lost, application is closed, drive is counterfeit) the blocks might not fully/properly restored even if the destructive option is not used.&lt;/p&gt;&lt;/li&gt;&lt;li&gt;Use cached data - Read *.h2w files that are already on the card. Does not write new h2w files.&lt;/li&gt;&lt;li&gt;Destructive Test - Speed up the test and minimize memory usage by skipping restoration of written blocks.&lt;/li&gt;&lt;/ul&gt;</source>
        <translation>&lt;h2&gt;操作模式&lt;/h2&gt;&lt;h3&gt;基础模式：&lt;/h3&gt;&lt;p&gt;使用 f3write 填充已挂载存储卡的空闲空间，再通过 f3read 读取内容以检查数据是否损坏，并确定存储卡的实际容量。&lt;/p&gt;&lt;h3&gt;高级模式（需要管理员权限）：&lt;/h3&gt;&lt;ul&gt;&lt;li&gt;快速测试 - 使用 f3probe 通过直接读写数据块来快速检查存储卡。未勾选此选项时，将挂载存储卡并使用 f3write/f3read 进行验证。&lt;/li&gt;&lt;li&gt;减少内存占用 - 执行速度较慢但内存用量更低的测试，包含数据块恢复功能。&lt;p&gt;&lt;b&gt;注意&lt;/b&gt;：某些情况下（如断电、程序异常退出关闭、设备质量问题），即使未使用破坏性选项，数据块也可能无法被完全恢复。&lt;/p&gt;&lt;/li&gt;&lt;li&gt;使用缓存数据 - 读取存储卡上已存在的 *.h2w 文件，但不要写入新的 h2w 文件。&lt;/li&gt;&lt;li&gt;破坏性测试 - 在写入测试数据后跳过恢复原始数据的操作，从而加速测试并最大限度减少内存占用。&lt;/li&gt;&lt;/ul&gt;</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="14"/>
        <source>Fight Flash Fraud (Qt)</source>
        <translation>Fight Flash Fraud (Qt)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="74"/>
        <location filename="../mainwindow.cpp" line="547"/>
        <source>Advanced Mode</source>
        <translation>高级模式</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="100"/>
        <source>Help</source>
        <translation>帮助</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="136"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:12pt;&quot;&gt;Please input the device path (mount point):&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:12pt;&quot;&gt;请输入要检查的设备路径（挂载点）：&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="174"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:12pt;&quot;&gt;Input or select a device (e.g. /dev/sdb):&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;span style=&quot; font-size:12pt;&quot;&gt;输入或选择一个设备（例如 /dev/sdb）：&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="204"/>
        <source>Quick Test</source>
        <translation>快速测试</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="214"/>
        <source>Use less memory</source>
        <translation>使用更少的内存</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="227"/>
        <source>Use cached data</source>
        <translation>使用缓存的数据</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="237"/>
        <source>Destructive Test</source>
        <translation>破坏性测试</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="272"/>
        <location filename="../mainwindow.cpp" line="70"/>
        <source>Progress:</source>
        <translation>进度：</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="350"/>
        <source>Click to hide test result</source>
        <translation>隐藏测试结果</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="427"/>
        <source>Space</source>
        <translation>空间</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="453"/>
        <source>Speed</source>
        <translation>速度</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="491"/>
        <location filename="../mainwindow.cpp" line="292"/>
        <source>Check!</source>
        <translation>测试！</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="517"/>
        <source>Exit</source>
        <translation>退出</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="67"/>
        <source>Ready</source>
        <translation>就绪</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="97"/>
        <source>%p% Available</source>
        <translation>%p% 可用</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="101"/>
        <source>Capacity not available</source>
        <translation>无法获取设备容量</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="165"/>
        <location filename="../mainwindow.cpp" line="174"/>
        <source>Quit F3</source>
        <translation>退出 F3</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="166"/>
        <source>The program is still running a check.
Quit anyway?</source>
        <translation>当前仍在执行检查任务。是否确定退出？</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="175"/>
        <source>You are going to abort this test.
Quit now will cause PERMANENT DATA LOSS!
Do you really want to quit?</source>
        <translation>您将要终止本次测试。
现在退出将会导致设备上的数据永久丢失！
您是否真的希望退出？</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="187"/>
        <source>Wrong Capacity Detected</source>
        <translation>检测到容量错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="188"/>
        <source>This device does not have the capacity it claims.
Would you like to fix it?</source>
        <translation>此设备的真实容量与其报告容量不符。
您是否要修复它？</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="194"/>
        <source>Fix Capacity</source>
        <translation>修复容量</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="195"/>
        <source>You are going to fix the capacity of this disk.
All data on this disk will be LOST!
Are you sure to continue?</source>
        <translation>您将要修复此磁盘的容量。
此磁盘上的所有数据都将丢失！
您确定要继续吗？</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="226"/>
        <source>Finished (without error).</source>
        <translation>已完成（无错误报告）。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="228"/>
        <source>Finished.</source>
        <translation>已完成。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="231"/>
        <source>Fixed successfully</source>
        <translation>已成功修复</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="232"/>
        <source>The capacity of the disk has been adjusted to what it should be.</source>
        <translation>此磁盘的容量已被调整为真实容量。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="251"/>
        <source>Stopped.</source>
        <translation>已停止。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="237"/>
        <source>Free Space: %1
Actual: %2</source>
        <translation>可用空间：%1
实际空间：%2</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="241"/>
        <source>Read speed: %1
Write speed: %2</source>
        <translation>读取速度：%1
写入速度：%2</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="257"/>
        <source>Progress: (Stage %1)</source>
        <translation>进度：（第%1阶段）</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="281"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="306"/>
        <source>No f3 program</source>
        <translation>缺少f3程序</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="307"/>
        <source>Cannot find f3read/f3write.
Please install f3 first.</source>
        <translation>找不到 f3read 和 f3write 命令行程序。
请先安装 f3。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="311"/>
        <source>No progress showing</source>
        <translation>无法显示进度</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="312"/>
        <source>You are using an old version of f3read/f3write.
The progress will not be shown during checking.</source>
        <translation>您正在使用老版本的 f3read 和 f3write。
检查过程中将无法显示进度。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="318"/>
        <location filename="../mainwindow.cpp" line="352"/>
        <source>Path error</source>
        <translation>路径错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="319"/>
        <source>Device path not found.
Please try mounting it correctly.</source>
        <translation>找不到指定的设备路径。
请确保您已正确挂载该设备。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="323"/>
        <source>Permission denied</source>
        <translation>权限错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="324"/>
        <source>Cannot write to device.
Try to re-run with sudo.</source>
        <translation>无法写入到该设备。
请以管理员权限（sudo）重新运行程序。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="326"/>
        <location filename="../mainwindow.cpp" line="333"/>
        <source>No enough space for test.</source>
        <translation>可用于测试的空间不足。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="329"/>
        <source>No space</source>
        <translation>空间不足</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="336"/>
        <source>Legacy Mode Only</source>
        <translation>仅支持传统模式</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="345"/>
        <source>Out of memory</source>
        <translation>内存耗尽</translation>
    </message>
    <message>
        <source>No enough memory for checking.
You may try again with following options:
  -Use less memory
  -Destructive Test</source>
        <translation type="vanished">可用于测试的内存不足。
您可以尝试以下方法：
 - 减少其他程序的内存占用
 - 执行破坏性测试</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="357"/>
        <location filename="../mainwindow.cpp" line="363"/>
        <source>Device type error</source>
        <translation>设备类型错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="358"/>
        <source>The device specified is not a disk.
Please make sure what you choose is a disk, not a partition.</source>
        <translation>指定的设备不是磁盘。
请确保您选择的是一个磁盘，而非磁盘上的分区。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="364"/>
        <source>The device specified is not a USB device.
Currently f3 does not support quick test on device that is not backed by USB (e.g. mmc, scsi).</source>
        <translation>指定的设备不是USB设备。
f3 当前不支持在非 USB 设备（例如 mmc、scsi 设备）上执行快速测试。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="371"/>
        <source>Probing Only</source>
        <translation>只检测</translation>
    </message>
    <message>
        <source>No enough space for checking.
Please delete some files, or format the device and try again.</source>
        <translation type="vanished">可用于检查的磁盘空间不足。
请先删除一部分文件，或者格式化设备后重试。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="211"/>
        <source>Ready.</source>
        <translation>就绪。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="214"/>
        <source>Checking... Please wait...</source>
        <translation>正在检查...请稍后...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="330"/>
        <source>No enough space for test.
Please delete some files, or format the device and try again.</source>
        <translation>可用于测试的磁盘空间不足。
请先删除一部分文件，或者格式化设备后重试。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="337"/>
        <source>f3probe is not found.
We are not able to run quick tests.</source>
        <translation>找不到 f3probe。
我们无法执行快速测试。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="341"/>
        <source>No cached data found. We will try to write data first...</source>
        <translation>找不到缓存数据。我们将先写入数据以供后续测试...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="346"/>
        <source>No enough memory for checking.
You may try again with following options:
  - Use less memory
  - Destructive Test</source>
        <translation>可用于测试的内存不足。
您可以尝试以下方法：
 - 减少其他程序的内存占用
 - 执行破坏性测试</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="353"/>
        <source>The path that you specified is not a directory.</source>
        <translation>您所指定的路径不是目录。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="372"/>
        <source>f3fix is not found.
We are not able to fix the disk if its reported capacity is wrong.</source>
        <translation>找不到 f3fix。
如果该磁盘的容量有误，我们将无法修复它。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="377"/>
        <source>No test result</source>
        <translation>没有测试结果</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="378"/>
        <source>No test has been completed. Please run a test first.</source>
        <translation>尚未完成任何测试。请先执行一个测试。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="382"/>
        <source>Fix failed</source>
        <translation>修复失败</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="383"/>
        <source>Cannot use the detected capacity for fixing.
You may need to report this as a bug.</source>
        <translation>无法将设备容量修正为检测到的真实容量。
您可能需要报告此软件缺陷。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="388"/>
        <source>Device inaccessible</source>
        <translation>设备无法访问</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="389"/>
        <source>Cannot access the specified device.
You may not have the right permission to read and write to it, or it has been damaged.</source>
        <translation>无法访问指定的设备。
您可能没有读取和写入设备的权限，或它已损坏。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="395"/>
        <source>Unknown error</source>
        <translation>未知错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="396"/>
        <source>Unknown error occurred.
You may need to report this as a bug.</source>
        <translation>发生了未知错误。
您可能需要报告此软件缺陷。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="398"/>
        <source>An error occurred.</source>
        <translation>发生了一个错误。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="417"/>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="418"/>
        <source>Please input the device path!</source>
        <translation>请输入设备路径！</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="436"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="437"/>
        <source>Cannot mount selected device!
You may need to run this program as root.</source>
        <translation>无法挂载选定的设备！
您可能需要以管理员权限运行本程序。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="458"/>
        <source>Run destructive test</source>
        <translation>执行破坏性测试</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="459"/>
        <source>You choose to run destructive test.
The data on the disk will be PERMANENTLY LOST!
Continue?</source>
        <translation>您选择了执行破坏性测试。
磁盘上的所有数据都将永久丢失！
继续吗？</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="504"/>
        <location filename="../mainwindow.cpp" line="554"/>
        <source>Choose Device Path</source>
        <translation>选择设备路径</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="536"/>
        <source>Switched to advanced mode.</source>
        <translation>切换至高级模式。</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="539"/>
        <source>Basic Mode</source>
        <translation>基础模式</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="544"/>
        <source>Switched to basic mode.</source>
        <translation>切换至基础模式。</translation>
    </message>
</context>
</TS>
