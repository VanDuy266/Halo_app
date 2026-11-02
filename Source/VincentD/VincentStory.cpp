#include "VincentGame.h"
void AVincentGameMode::LoadStory(){
 Chapters.Reset();
 { FVincentChapter C;
 C.Name=TEXT("Phòng lưu trữ");
 C.Intro=TEXT("Bạn tỉnh dậy trong khu nghiên cứu D. Radio rè lên: “Vincent, nghe thấy tôi chứ? Tôi là An. Tôi bị nhốt ở tầng dưới. Đừng đi một mình. Hãy tìm hồ sơ số 03.” Bạn không nhớ mình đã đến đây bằng cách nào.");
 C.Radio=TEXT("AN / Ba chốt nằm ở khung tranh, đồng hồ và ngăn kéo. Ghép chữ số theo đúng thứ tự đó. Tôi sẽ chờ ở đầu bên kia.");
 C.Reveal=TEXT("Trong ngăn kéo là thẻ nhân viên: Vincent D — trưởng dự án JANUS. Bạn không phải một vị khách. An nói nhỏ: “Tôi biết. Nhưng tôi cần anh tin tôi thêm một lần nữa.”");
 C.Board=TEXT("| | | | | | |\nMỖI VẠCH / MỘT NGƯỜI");
 C.Note=TEXT("7 NGƯỜI ĐẾN\n5 NGƯỜI MẤT TÍCH");
 C.Hour=4;
 C.Color=FColor::FromHex(TEXT("d1ee3aFF")).ReinterpretAsLinear();
 C.Puzzles.Add({TEXT("Khung tranh"),TEXT("Mỗi vạch sáng trên khung tranh đại diện cho một người bước vào khu D. Hãy quan sát và đếm số người."),TEXT("7"),TEXT("7"),TEXT("Bảy vạch, bảy người. Chốt đầu tiên là 7."),TEXT("Có bảy vạch đứng sáng màu trên khung tranh.")});
 C.Puzzles.Add({TEXT("Đồng hồ"),TEXT("Kim dài chỉ 12. Đọc giờ mà kim ngắn đang chỉ trên đồng hồ trong phòng."),TEXT("4"),TEXT("4"),TEXT("04:00 — thời điểm JANUS khởi động. Chốt thứ hai là 4."),TEXT("Kim ngắn hướng xuống bên phải, vào số 4.")});
 C.Puzzles.Add({TEXT("Hồ sơ mất tích"),TEXT("Tờ hồ sơ ghi: “7 người đến. 5 người mất tích.” Nếu những người mất tích đã rời khu lưu trữ, còn lại bao nhiêu người?"),TEXT("2"),TEXT("2"),TEXT("Hai người còn lại: Vincent và An. Chốt cuối là 2."),TEXT("Lấy số người đến trừ số người mất tích: 7 − 5.")});
 Chapters.Add(C); }
 { FVincentChapter C;
 C.Name=TEXT("Phòng tín hiệu");
 C.Intro=TEXT("Thẻ D mở phòng tín hiệu. Máy chủ vẫn chạy dù tòa nhà đã bỏ hoang. Trên màn hình là một cuộc gọi cùng ngày hôm nay… ba năm trước. “Đừng tắt radio,” An nói. “Tín hiệu của tôi nằm trong đó.”");
 C.Radio=TEXT("AN / Đồng hồ ở đây chạy nhanh hai giờ. Mật thư Caesar trên bàn yêu cầu lùi từng chữ ba vị trí trong bảng A–Z. Tôi vẫn ở đây, Vincent.");
 C.Reveal=TEXT("Bạn khôi phục tín hiệu. JANUS đã giữ ý thức của An trong hệ thống sau tai nạn. Cô ấy không ở tầng dưới. Radio chứa phần ký ức cuối cùng còn sống của cô.");
 C.Board=TEXT("2  4  8  ?\nGIỮ CHỮ SỐ HÀNG ĐƠN VỊ");
 C.Note=TEXT("FKLD\nCAESAR / LÙI 3 CHỮ");
 C.Hour=5;
 C.Color=FColor::FromHex(TEXT("69d9e7FF")).ReinterpretAsLinear();
 C.Puzzles.Add({TEXT("Bảng chuỗi tín hiệu"),TEXT("Bảng hiển thị 2, 4, 8, ?. Mỗi số gấp đôi số trước. Nhập số tiếp theo; hệ thống giữ hàng đơn vị làm chốt khóa."),TEXT("16"),TEXT("6"),TEXT("Số tiếp theo là 16. Chữ số hàng đơn vị là 6."),TEXT("8 × 2 = 16. Nhập số 16.")});
 C.Puzzles.Add({TEXT("Đồng hồ sai lệch"),TEXT("Đồng hồ chỉ 5 giờ nhưng chạy nhanh đúng 2 giờ. Giờ thật là mấy giờ?"),TEXT("3"),TEXT("3"),TEXT("05:00 − 2 giờ = 03:00. Chốt thứ hai là 3."),TEXT("Đồng hồ chạy nhanh nên phải trừ hai giờ.")});
 C.Puzzles.Add({TEXT("Mật thư Caesar"),TEXT("Giải mã FKLD bằng cách lùi mỗi chữ 3 vị trí trong bảng A–Z (A lùi 1 thành Z). Nhập từ thu được, không dấu. Tờ giấy ghi thêm: “Giải đúng lời nhắn sẽ mở kênh 1.”"),TEXT("chia"),TEXT("1"),TEXT("FKLD → CHIA. “Chia sẻ tín hiệu, đừng cắt nó.” Kênh 1 đã mở. Chốt thứ ba là 1."),TEXT("F → C, K → H, L → I, D → A. Viết liền bốn chữ.")});
 Chapters.Add(C); }
 { FVincentChapter C;
 C.Name=TEXT("Lõi JANUS");
 C.Intro=TEXT("Lõi JANUS nằm sau cánh cửa cuối. Một bộ nhớ di động trống trên bàn. Bạn có thể đưa An ra ngoài, nhưng trước hết phải giải cơ chế bảo vệ và quyết định điều gì đáng cứu.");
 C.Radio=TEXT("AN / Tọa độ B2: chữ là cột, số là hàng. Số nhị phân 1010 có các trọng số 8, 4, 2, 1. Nếu còn cơ hội… đừng bỏ tôi ở đây.");
 C.Reveal=TEXT("Ba chốt lõi đã mở. Bạn chỉ có một bộ nhớ, đủ mang theo ý thức của An hoặc hồ sơ để vạch trần JANUS. Cửa thoát đang chờ.");
 C.Board=TEXT("    A   B   C\n1   5   2   7\n2   4   8   1\n3   6   0   3");
 C.Note=TEXT("TRẠNG THÁI : 1010\nNHỊ PHÂN → THẬP PHÂN");
 C.Hour=9;
 C.Color=FColor::FromHex(TEXT("efb676FF")).ReinterpretAsLinear();
 C.Puzzles.Add({TEXT("Bảng tọa độ"),TEXT("Bảng có cột A, B, C. Hàng 1: 5, 2, 7. Hàng 2: 4, 8, 1. Hàng 3: 6, 0, 3. Tìm giá trị tại cột B, hàng 2."),TEXT("8"),TEXT("8"),TEXT("Giá trị ở B2 là 8. Chốt thứ nhất mở."),TEXT("Hàng thứ hai, cột ở giữa: số 8.")});
 C.Puzzles.Add({TEXT("Đồng hồ lõi"),TEXT("Đọc giờ đang hiển thị trên đồng hồ lõi. Kim dài chỉ 12; kim ngắn chỉ ngang sang trái."),TEXT("9"),TEXT("9"),TEXT("09:00. Đồng hồ của An đã chạy lại. Chốt thứ hai là 9."),TEXT("Kim ngắn hướng ngang sang trái là số 9.")});
 C.Puzzles.Add({TEXT("Thiết bị nhị phân"),TEXT("Chuyển 1010 từ nhị phân sang thập phân. Các trọng số từ trái sang phải: 8, 4, 2, 1. Cộng các vị trí có bit 1. Hệ thống giữ hàng đơn vị cho chốt cuối."),TEXT("10"),TEXT("0"),TEXT("1010₂ = 8 + 2 = 10. Hàng đơn vị là 0."),TEXT("Hai bit bật có trọng số 8 và 2. Tổng bằng 10.")});
 Chapters.Add(C); }
}
