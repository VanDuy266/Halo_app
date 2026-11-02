# Đặc tả chính — Vincent D

Studio: **VINCENT D STUDIO**. Tuyến truyện: **Từ Xóm Nhỏ Đến Thành Phố Lớn**.

## Những điểm người dùng đã chốt

Duy, **21 tuổi**, bắt đầu ở một xóm nhỏ với xe máy cũ, phòng ở đơn giản và ít tiền. Người chơi chọn ở lại phát triển quê hương hoặc lên thành phố lập nghiệp. Quyết định ảnh hưởng đến tiền, quan hệ, sự nghiệp và diễn biến truyện.

Các nhóm tính năng mục tiêu:

1. Phòng 3D: thuê/mua nhà, trang trí, PC gaming, ngủ và sinh hoạt.
2. Xe: xe máy, ô tô, xe điện và xe thể thao; lái, mua bán, độ và sửa xe.
3. Thành phố: chung cư, trung tâm thương mại, bệnh viện, trường học, cửa hàng và giao thông đông đúc.
4. Xóm: nhà dân, hẻm, quán cà phê, chợ, NPC sinh hoạt và trò chuyện.
5. Việc làm: làm thuê, giao hàng, kinh doanh, mở cửa hàng và thành lập công ty.
6. NPC: lịch sinh hoạt, tính cách, trí nhớ và quan hệ riêng.
7. Thế giới: thời tiết, ngày đêm, giao thông, sự kiện bất ngờ và nhiệm vụ ngẫu nhiên.

Trọng tâm: cuộc sống có câu chuyện, bí mật khu phố và hậu quả lâu dài; không chỉ tối ưu kiếm tiền.

Mục tiêu nghệ thuật: **Unreal Engine 5**, nhân vật chân thực, ánh sáng điện ảnh, chuyển động tự nhiên và bản đồ rộng. Màn chơi chiếm toàn cửa sổ; hướng dẫn, bản đồ và nhật ký chỉ là overlay khi cần. Khởi động bản Unreal: giới thiệu Unreal Engine → logo Vincent D Studio → menu → chơi.

## Những điều không tự suy diễn thành yêu cầu bắt buộc

- Chưa yêu cầu multiplayer hoặc dịch vụ tài khoản online.
- “NPC thông minh” không bắt buộc dùng mô hình ngôn ngữ hay dịch vụ AI bên ngoài. Lịch, trạng thái, cờ hội thoại và trí nhớ lựa chọn có thể được lập trình bằng C++/AI của Unreal.
- “Bản đồ rộng” không đồng nghĩa phải tải mọi khu và mọi NPC cùng lúc.
- Hình tham chiếu là mục tiêu nghệ thuật, không phải ảnh chứng minh source hiện có đã đạt chất lượng đó.
- Tuổi, tên Duy và định hướng Việt Nam là yêu cầu đã chốt. Tên xóm/thành phố/NPC phụ và bí mật cụ thể trong tài liệu kịch bản là đề xuất, chưa phải lời người dùng xác nhận từng chi tiết.

## Bản chơi đầu tiên cần hoàn thiện

Một ngày của Duy trong xóm: **phòng riêng → quán ăn → tiệm sửa xe → chuyến giao hàng → cuộc hẹn ở bến xe**.

Bản này cần một phòng hoàn thiện hình ảnh, một tuyến hẻm, ba NPC có lịch cơ bản, một xe máy, tiền/chi tiêu, năm nhiệm vụ và lưu/tải lại. Chưa cần xây đồng thời biển, núi, bệnh viện, công ty, ô tô và một thành phố hoàn chỉnh.

Đây là phạm vi triển khai đề xuất để kiểm chứng vòng chơi và chất lượng, không phải tuyên bố các hệ thống đã có.

## Bí mật khu phố — đề xuất liên kết cốt truyện

Trong phòng cũ, Duy tìm thấy một quyển sổ ghi chi phí của dự án sửa khu sinh hoạt cộng đồng năm xưa. Một khoản tiền được ghi là đã thanh toán, nhưng công trình chưa bao giờ hoàn thành. Người trong xóm kể những phiên bản khác nhau: có người nghĩ ai đó lấy tiền, có người cho biết tiền được chuyển sang hỗ trợ một gia đình gặp nạn.

Người chơi gặp người liên quan thông qua công việc thường ngày: giao đồ cho chủ quán cũ, sửa xe cho người từng giữ sổ, tìm một địa chỉ mới ở thành phố. Bằng chứng bổ sung khiến câu chuyện bớt đơn giản hơn một vụ “tìm kẻ xấu”. Một số nhân vật đã chọn im lặng để giữ thể diện cho người khác; một số người hưởng lợi từ hiểu lầm.

Lựa chọn cuối không chỉ là công khai hay che giấu: Duy có thể kiểm chứng thêm, tổ chức đối thoại, thương lượng hoàn trả hoặc công khai tài liệu. Những người bị ảnh hưởng có phản ứng và quan hệ khác nhau. Tuyến này vẫn có thể tiến triển dù Duy làm nghề sửa xe, giao hàng hay kinh doanh, và không buộc người chơi bỏ thành phố hoặc bỏ quê.

Mọi nhiệm vụ quan trọng cần tách rõ điều Duy biết, điều NPC biết, bằng chứng đã có và hệ quả được lưu. Không để NPC “biết mọi lựa chọn” nếu họ chưa có cách biết thông tin.

## Mẫu một lựa chọn có hậu quả

Khải đề nghị Duy nhận một hợp đồng giao hàng trùng giờ chuẩn bị chợ cuối tuần.

- Nhận hợp đồng: thêm tiền/quan hệ nghề nghiệp; cộng đồng thiếu người nếu Duy không báo trước hoặc tìm người thay.
- Giữ lời hẹn với xóm: tăng tin cậy; có thể lỡ hợp đồng, nhưng được giới thiệu khách hàng địa phương.
- Thương lượng giờ hoặc thuê người phụ: mất một phần tiền/cần kỹ năng và quan hệ, đổi lại giữ được hai đầu việc.

Không có đáp án mặc định “luôn tốt nhất”. Hệ quả dựa trên cách hành động và thông tin người chơi đã có, không trừng phạt bất ngờ bằng cờ ẩn không được gợi ý.

## Trạng thái hiện tại

Đã có tài liệu định hướng và kịch bản mở đầu. Source Unreal đã xuất trước đây vẫn là prototype thoát phòng, chưa phải life simulator này. Chưa build được `.exe` life simulator, chưa kiểm chứng ảnh Unreal đạt mức ảnh tham chiếu. Việc chuyển đổi cần triển khai và kiểm thử các hệ thống mới, không chỉ đổi tên menu.
