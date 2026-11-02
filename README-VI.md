# Vincent D: Cùng Nhau Bỏ Trốn — Unreal C++

## Bản chính đã chuyển sang C++

Gameplay không chạy bằng Python, JavaScript hoặc C#. Module `VincentD` chứa nhân vật góc nhìn thứ nhất, tương tác, câu đố, lưu game, giao diện UMG và intro. Module `VincentDTools` là công cụ C++ chỉ dành cho Unreal Editor để nhập vật liệu/logo và mesh vát cạnh. Không cần plugin Python.

**Trạng thái kiểm chứng:** môi trường tạo source không có Unreal Engine, Windows SDK hoặc trình biên dịch Unreal. Chưa chạy Unreal Header Tool, chưa biên dịch C++, chưa mở Editor và chưa xuất `.exe`. Kiểm tra cấu trúc source không thay thế kiểm tra build. Bản web đã thử ở trình duyệt là một bản thử riêng, không phải bằng chứng bản Unreal đã chạy.

## Nội dung trong source

- Góc nhìn thứ nhất, di chuyển WASD/phím mũi tên, nhìn bằng chuột, line trace tương tác bằng E.
- Ba chương, chín câu đố và hai kết thúc; SaveGame slot `VincentD`.
- Sơ đồ biệt thự sáu khu có cửa thông nhau: sảnh, phòng khách, phòng ăn, thư viện, bếp và phòng ngủ. Geometry/collision được dựng bằng C++; cần kiểm tra trực tiếp trong Editor.
- Vật liệu PBR (base color / normal / roughness), mesh có cạnh vát, ánh sáng cửa sổ và đèn nội thất.
- Intro toàn màn hình: giới thiệu Unreal Engine → logo VINCENT D STUDIO → menu Vincent D → vào game. Nếu chưa có texture logo Unreal chính thức đã nhập vào dự án, dùng chữ “UNREAL ENGINE”, không vẽ lại hoặc sửa nhãn hiệu engine.
- Gameplay toàn cửa sổ, không có trang giới thiệu hoặc bảng hướng dẫn bao quanh. Menu/radio là giao diện trong game.

Một số model công khai ở `SourceAssets/Models` và `SourceAssets/Lab` được giữ để tiếp tục nhập bằng Interchange của Unreal. Các model này đã được áp dụng trong bản thử trình duyệt, **chưa được xác nhận nhập/đặt tự động trong bản C++**. Native level vẫn dùng các cấu kiện C++ và mesh vát cạnh cho phần lớn nội thất. Không mô tả bản này là đã có chất lượng photoreal hoặc đã ngang một game simulator thương mại.

## Cài trên Windows

1. Cài Unreal Engine 5 qua Epic Games Launcher. Chọn phiên bản đã cài khi mở `.uproject`; chưa xác minh tương thích với phiên bản cụ thể trên máy bạn.
2. Cài Visual Studio 2022 với **Game development with C++**, MSVC v143 và Windows SDK. Nếu engine yêu cầu toolset khác, cài đúng phiên bản được thông báo trong Visual Studio Installer, không tắt kiểm tra toolchain.
3. Giải nén toàn bộ dự án vào đường dẫn ngắn, ví dụ `C:\Games\VincentD-Unreal`.
4. Chuột phải `VincentD.uproject` → Switch Unreal Engine version → chọn engine. Generate Visual Studio project files.
5. Mở solution, chọn **Development Editor / Win64**, build `VincentDEditor`. Nếu lỗi, đọc lỗi đầu tiên trong Output; source này chưa được biên dịch ở môi trường tạo file.

Nguồn tải công cụ: https://www.unrealengine.com/download và https://visualstudio.microsoft.com/vs/ .

## Chuẩn bị vật liệu/logo bằng C++

Sau khi build Editor thành công và mở dự án:

1. Mở **Output Log** trong Unreal Editor.
2. Nhập lệnh:

```text
Vincent.PrepareAssets
```

Lệnh nhập 18 texture gốc từ `SourceAssets/Textures`, tạo sáu material trong `/Game/Vincent/Materials`, nhập `SM_BevelCube.obj` và hai PNG logo do Vincent D Studio sử dụng. Tài sản có sẵn được giữ nguyên, không ghi đè.

3. Kiểm tra log có lỗi hay không rồi **Save All**. Material, mesh và logo cần xuất hiện trong Content Browser; lệnh in thông báo kết thúc không tự chứng minh mọi bước đã thành công.
4. Nếu có PNG logo Unreal chính thức, đặt tại `SourceAssets/Branding/UnrealEngineLogo.png` trước khi chạy lệnh. File `.uasset` logo đóng gói từ game khác không được coi là PNG và không được tự sửa binary.
5. Các GLB bổ sung có thể kéo từ `SourceAssets/Models` hoặc `SourceAssets/Lab` vào Content Browser bằng trình nhập Interchange của engine. Giữ attribution trong `THIRD-PARTY.md`. Việc gắn NPC/xe và vật liệu của các model đó vào gameplay Unreal còn cần thao tác nhập/cấu hình và kiểm thử, không được coi là đã hoàn tất chỉ vì có file nguồn.

Texture đã được tạo và lưu sẵn. Không cần chạy Python để tái tạo hoặc chơi game. Mã sinh texture ban đầu chỉ giữ ở bản web như tài liệu nguồn tham khảo, ngoài dự án Unreal.

## Chạy thử

Chọn **Play → Standalone Game**. Kiểm tra intro/menu, di chuyển, cửa phòng, collision, đọc/nhập câu đố, đáp án sai, khóa điều kiện, cả ba chương, SaveGame, chơi lại và hai kết thúc. Escape trong Play In Editor có thể bị Editor dùng để dừng chơi; nên thử chế độ Standalone.

Map mặc định `/Engine/Maps/Entry` dùng GameMode C++ để dựng thế giới. Nếu bản engine không cook map đó, tạo Empty Level, lưu `/Game/Maps/Main`, đổi GameDefaultMap, EditorStartupMap và MapsToCook, đồng thời đổi `-map` trong script build. Chưa kiểm tra phương án này trong Editor tại môi trường hiện tại.

## Xuất Windows `.exe`

Sau khi Editor build/chơi đạt và `Vincent.PrepareAssets` thành công, mở PowerShell tại thư mục dự án:

```powershell
.\Build-Windows.ps1 -EngineRoot 'C:\Program Files\Epic Games\UE_5.8'
```

Thay đường dẫn ví dụ bằng thư mục engine thực tế. Script kiểm tra material đã chuẩn bị, gọi Unreal Automation Tool để build/cook/stage/package/archive Win64 Shipping. Nếu PowerShell không cho chạy script, dùng Unreal Editor → Platforms → Windows → Package Project; không cần tắt bảo vệ toàn hệ thống.

Kết quả dự kiến nằm trong `Packaged`. Kiểm tra `VincentD.exe` ở đó cùng toàn bộ dữ liệu đi kèm. Chưa có `.exe` nào được tạo trong môi trường này.

Muốn cho người khác tải chơi: ZIP **toàn bộ thư mục đầu ra**, không chỉ `.exe`, rồi tải lên itch.io hoặc GitHub Releases. Người chơi không cần cài Unreal Engine. Link công khai chỉ tạo được sau khi có bản Windows đã build và thử thành công.
