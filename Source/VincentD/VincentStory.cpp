#include "VincentGame.h"

FString AVincentGameMode::OpeningText()const{
 if(Progress.Current!=VincentLife::Quest::Home)return TEXT("Đã khôi phục cuộc sống của Duy từ bản lưu. Xem điện thoại để kiểm tra tiền và nhiệm vụ hiện tại.\n\n")+ObjectiveText();
 return TEXT("Duy, 21 tuổi, trở về xóm An Bình với một căn phòng nhỏ, chiếc xe máy cũ và 250.000 đồng. Mẹ Hạnh đang cần người phụ quán sáng; chú Bảy hẹn kiểm tra chiếc xe. Bạn có thể ở quê gây dựng cuộc sống hoặc lên Hải Minh tìm cơ hội. Trước mắt, hãy xem căn phòng và bắt đầu ngày đầu tiên. Bản hiện tại là lát cắt gameplay thử nghiệm, chưa phải thế giới mở hoàn chỉnh.");
}
FString AVincentGameMode::ObjectiveText()const{
 using VincentLife::Quest;
 switch(Progress.Current){
 case Quest::Home:return TEXT("Về căn phòng, tương tác với bàn để bắt đầu ngày đầu tiên. Có thể xem quyển sổ cũ cạnh đó.");
 case Quest::Breakfast:return TEXT("Đến Quán mẹ Hạnh. Nhận đơn và phục vụ đúng hai tô để hoàn thành ca sáng.");
 case Quest::Scooter:return TEXT("Đến Tiệm chú Bảy. Đèn xe yếu, tiếng đề lạch cạch: chẩn đoán bộ phận cần thay. Phí sửa 50.000 đồng.");
 case Quest::Delivery:return TEXT("Mang gói hàng D-03 từ chú Bảy tới Nhà số 17. Người nhận sẽ kiểm tra mã trên phiếu giao.");
 case Quest::Decision:return TEXT("Đến bến xe, đọc tin nhắn của Minh rồi chọn ở lại phát triển xóm hoặc đặt lịch lên thành phố.");
 case Quest::FreePlay:return Progress.Direction==VincentLife::Path::Village?TEXT("Bạn chọn bắt đầu tại xóm. Có thể về phòng nghỉ, ăn uống hoặc mua một chậu cây. Thành phố chưa được dựng trong bản này."):TEXT("Bạn đã đặt lịch lên Hải Minh. Có thể tiếp tục sinh hoạt ở xóm; khu thành phố sẽ thuộc phần triển khai tiếp theo.");
 }
 return TEXT("");
}
