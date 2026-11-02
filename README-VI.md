# Vincent D — Từ Xóm Nhỏ Đến Thành Phố Lớn

Studio: VINCENT D STUDIO. Nhân vật chính: Duy, 21 tuổi. Bản source 0.2 chuyển vòng chơi từ thoát phòng sang mô phỏng cuộc sống Việt Nam.

## Đã triển khai trong source C++

- Góc nhìn thứ nhất, di chuyển/nhìn và tương tác bằng E; giao diện nằm trong cửa sổ game.
- Intro Unreal → Vincent D Studio → menu. Title của game dùng chữ mới; logo game thoát phòng cũ không được sử dụng.
- Luồng ngày đầu: xem phòng → phụ quán mẹ → chẩn đoán/sửa xe → giao kiện D-03 tại nhà số 17 → chọn ở xóm hoặc hẹn lên Hải Minh.
- Tiền công, ca làm thêm một lần mỗi ngày để có đường kiếm lại tiền, chi phí sửa, kiểm tra điều kiện nhiệm vụ, thể lực/mức no, ăn/ngủ, mua một chậu cây, mức tin cậy với ba nhân vật và manh mối quyển sổ.
- Điện thoại/nhật ký trong menu; thời gian dừng khi đang đọc giao diện.
- SaveGame riêng `VincentLife_v1`; không đọc hoặc xóa save thoát phòng cũ.
- EngineAssociation 5.8, Target BuildSettings V7 / Unreal5_8 theo log máy người dùng. Sửa UMG padding qua UButtonSlot.

## Kiểm chứng và giới hạn

Phần luật độc lập ở `LifeProgression.h` đã được biên dịch bằng g++ và chạy kiểm thử trong `Tests/LifeProgressionTests.cpp`: thứ tự nhiệm vụ, đáp án sai, nhận thưởng một lần, chi phí, thiếu tiền/thể lực, ăn/ngủ, vòng đời kiện hàng, hai lựa chọn, thời gian và kiểm tra trạng thái lưu.

**Chưa chạy UnrealHeaderTool, biên dịch module bằng Unreal, thử Editor hoặc đóng gói Windows trong môi trường này.** Kiểm thử luật C++ không chứng minh phần UMG/Unreal đã build được. Chưa có `.exe` bản mới được kiểm chứng.

Bản đồ hiện là bố cục thử gồm phòng và các điểm tương tác ở xóm, dựng bằng cấu kiện C++. Chưa đạt hình ảnh chân thực của ảnh tham chiếu. Hội thoại NPC hiện qua điểm tương tác, chưa có NPC hoạt ảnh/lịch sinh hoạt. Xe mới có tình huống sửa và trạng thái sẵn sàng, **chưa có lái xe/vật lý xe**; nhiệm vụ giao hàng hiện đi bộ. Lựa chọn thành phố lưu hướng đi và lời hẹn, chưa có bản đồ thành phố. Chưa có trang trí kéo/thả, kinh doanh đầy đủ, thời tiết hoặc đám đông giao thông.

Bộ mô hình từ ZIP `PhuQuocCity_3D_Source.zip` đã được loại khỏi dự án đang dùng theo yêu cầu; file gốc được giữ riêng trong workspace. Source không cần Python để chạy game.

## Mở trên máy Windows đang có UE 5.8

1. Tải bản ZIP mới từ nhánh `vincent-cpp-source` và giải nén vào thư mục mới; giữ bản đang làm nếu có thay đổi riêng.
2. Chuột phải `VincentD.uproject` → Show more options → Switch Unreal Engine version → 5.8.
3. Mở `.uproject`, đồng ý build nếu được hỏi. Cần Visual Studio với workload Game development with C++ và Windows SDK. Log cũ của máy người dùng đã nhận Win64 SDK; lỗi V4/shared build environment đã được chỉnh trong Target.
4. Nếu Visual Studio báo Unreal Engine Missing, mở trực tiếp bằng engine. Trong PowerShell tại thư mục dự án:

```powershell
.\Open-Unreal.ps1 -EngineRoot 'D:\FXServer\UE_5.8'
```

Đường dẫn trên lấy từ log người dùng, thay nếu engine đặt nơi khác. Có thể mở `D:\FXServer\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe`, rồi Browse tới `.uproject`. Nếu PowerShell chặn script, dùng cách mở Editor trực tiếp; không cần thay chính sách bảo vệ toàn máy.

5. Nếu cần build qua solution: Generate Visual Studio project files trên `.uproject`, mở `VincentD.sln`, chọn Development Editor / Win64 và build VincentDEditor. Không tạo Console App/Empty Project mới trong Visual Studio.
6. Nếu lỗi tiếp, đọc `%LOCALAPPDATA%\UnrealBuildTool\Log.txt` và gửi đoạn error đầu tiên. Không đổi compiler hoặc bật override build environment khi chưa có log chỉ rõ lý do.

## Chuẩn bị hình ảnh và chơi

Sau khi Editor mở được, mở Output Log và chạy `Vincent.PrepareAssets`, kiểm tra lỗi rồi Save All. Công cụ Editor C++ nhập các texture PBR, mesh vát cạnh và logo studio, giữ nguyên tài sản có sẵn. Hình học fallback vẫn chạy về mặt thiết kế khi thiếu material, nhưng chất lượng không tương đương tài sản đã chuẩn bị.

Play → Standalone Game. WASD/mũi tên để đi, chuột để nhìn, E để tương tác gần đồ vật, Escape để xem điện thoại/nhật ký. Điểm nhận việc và thông tin ở các bảng/quầy, chưa phải nhân vật cuối cùng. Đọc hộp thoại để biết dữ kiện trước khi xác nhận. Nút Tiếp tục đóng thoại; Bắt đầu lại yêu cầu xác nhận.

Đường kiểm tra: bàn trong phòng → Quán mẹ Hạnh (phục vụ 2 tô) → Tiệm chú Bảy (BINH DIEN, phí 50.000) → Nhà số 17 (D-03) → bến xe → chọn hướng đi. Có thể ăn, ngủ và mua cây bên ngoài luồng này; nhiệm vụ không trả thưởng lại khi đã hoàn thành.

## Kiểm thử luật C++ không cần Unreal

```sh
g++ -std=c++17 -Wall -Wextra -Werror Tests/LifeProgressionTests.cpp -o life-tests
./life-tests
```

Đây là kiểm thử logic; kiểm thử tích hợp trong Unreal vẫn bắt buộc trước khi gọi bản game chạy được.

## Đóng gói Windows

Sau khi build và chơi thử đạt:

```powershell
.\Build-Windows.ps1 -EngineRoot 'D:\FXServer\UE_5.8'
```

Script gọi Unreal Automation Tool cho Win64 Shipping và tìm VincentD.exe. Gửi toàn bộ thư mục kết quả (có dữ liệu/DLL), không chỉ riêng `.exe`. Người chơi không cần cài Unreal. Chưa có bản Windows mới được tạo trong phiên này.

## Định hướng tiếp theo

`Design/USER-VISION.md` giữ các yêu cầu chính; `Design/LIFE-VIET-DESIGN.md` có bốn hồi, hệ thống mục tiêu và hướng hình ảnh. Nội dung trong tài liệu định hướng chưa đồng nghĩa đã được triển khai. Model GLB công khai còn lại ở SourceAssets chỉ là tài nguyên tham khảo/để nhập tiếp, không tự động trở thành NPC hay xe đã điều khiển được. Xem THIRD-PARTY.md cho nguồn và giấy phép.
