# PHẦN 5: QUẢN LÝ GIỎ HÀNG BẰNG DANH SÁCH LIÊN KẾT ĐƠN VÀ THUẬT TOÁN MERGE SORT (TV5)

## 1. Mục tiêu
* Xây dựng module quản lý giỏ hàng trực tuyến (`src/cart.cpp`) cho phép thêm sản phẩm vào cuối giỏ, tìm kiếm theo mã sản phẩm, xóa từng món, xóa toàn bộ giỏ hàng và hiển thị hóa đơn chi tiết (tính tạm tính, thuế VAT 8%, áp dụng giảm giá Voucher).
* Cài đặt thuật toán **Merge Sort trực tiếp trên Danh sách liên kết đơn** (`sortCartBySubtotal`) để sắp xếp các mặt hàng trong giỏ theo **Thành tiền tăng dần**, đảm bảo tính ổn định (Stable) và không sử dụng mảng phụ.
* Xử lý triệt để các trường hợp biên (giỏ rỗng, đơn giá âm do dữ liệu giảm giá lỗi, voucher vượt tổng thanh toán, giải phóng bộ nhớ an toàn).

---

## 2. Cấu trúc dữ liệu sử dụng
Giỏ hàng được cài đặt bằng **Danh sách liên kết đơn (Singly Linked List)** quản lý thông qua con trỏ đầu `head`:

* **`struct CartItem`**: Lưu thông tin một mặt hàng trong giỏ gồm `productId` (mã SP), `name` (tên SP), `unitPrice` (đơn giá sau khuyến mãi), `quantity` (số lượng mua) và phương thức `subtotal()` trả về thành tiền (`unitPrice * quantity`).
* **`struct CartNode`**: Một nút trong danh sách liên kết, gồm trường dữ liệu `data` kiểu `CartItem` và con trỏ `next` trỏ tới nút kế tiếp.
* **`struct CartLinkedList`**: Quản lý toàn bộ danh sách thông qua con trỏ `head` (trỏ tới nút đầu tiên, bằng `nullptr` khi giỏ hàng rỗng).

**Lý do lựa chọn DSLK đơn:** Số lượng mặt hàng trong giỏ thay đổi liên tục (thêm, xóa linh hoạt) và không biết trước kích thước tối đa. DSLK đơn cấp phát động từng nút qua `new`/`delete`, không gây lãng phí bộ nhớ và thao tác xóa nút không cần dịch chuyển các phần tử như mảng.

---

## 3. Giải thuật và đánh giá độ phức tạp

### 3.1. Các thao tác cơ bản trên giỏ hàng
* **Khởi tạo (`initCart`) & Xóa đầu (`deleteHeadCart`):** Gán/cập nhật trực tiếp con trỏ `head`, độ phức tạp **$O(1)$**.
* **Thêm cuối (`insertTailCart`):** Duyệt từ `head` đến nút cuối cùng (`last->next == nullptr`) để nối nút mới vào cuối, giữ đúng thứ tự khách chọn hàng. Độ phức tạp **$O(n)$**.
* **Tìm kiếm (`findCartNode`) & Xóa theo ID (`removeProductFromCart`):** Duyệt tuần tự qua từng nút (sử dụng thêm con trỏ `prev` khi xóa nút giữa/cuối để nối lại liên kết trước khi `delete`). Độ phức tạp **$O(n)$**.
* **Xóa toàn bộ (`clearCart`):** Lặp gọi `deleteHeadCart` cho đến khi `head == nullptr`, thu hồi toàn bộ vùng nhớ động. Độ phức tạp **$O(n)$**.

### 3.2. Thuật toán Merge Sort trên Danh sách liên kết đơn (`sortCartBySubtotal`)
Thuật toán chia để trị gồm 3 bước thao tác trực tiếp bằng con trỏ:
1. **Tìm điểm giữa và tách đôi (`getMiddleNode`):** Sử dụng kỹ thuật **hai con trỏ nhanh – chậm (`slow` / `fast`)**. Con trỏ `slow` đi 1 bước, `fast` đi 2 bước; khi `fast` chạm cuối danh sách thì `slow` đứng tại nút giữa. Ngắt liên kết `mid->next = nullptr` để chia danh sách thành 2 nửa độc lập.
2. **Sắp xếp đệ quy (`mergeSortRec`):** Gọi đệ quy sắp xếp nửa bên trái và nửa bên phải cho đến trường hợp cơ sở (danh sách rỗng hoặc chỉ có 1 nút).
3. **Trộn hai danh sách đã sắp xếp (`mergeSortedLists`):** So sánh `subtotal()` của hai nút đầu danh sách trái và phải, nối lại con trỏ `next` theo thứ tự tăng dần. Sử dụng điều kiện **`<=`** (`left->data.subtotal() <= right->data.subtotal()`) để ưu tiên giữ nút bên trái đứng trước khi hai món bằng thành tiền, đảm bảo **tính ổn định (Stable)**.

**Đánh giá độ phức tạp:**
* **Thời gian:** Chia đôi danh sách mất $O(\log n)$ mức đệ quy, mỗi mức duyệt trộn hết $n$ nút mất $O(n)$ $\Rightarrow$ Tổng độ phức tạp là **$O(n \log n)$** trong mọi trường hợp (tốt nhất, trung bình, xấu nhất).
* **Không gian:** Không cấp phát mảng phụ, chỉ tốn **$O(\log n)$** bộ nhớ ngăn xếp cho lời gọi đệ quy.

---

## 4. Các trường hợp đặc biệt đã xử lý
1. **Giỏ hàng rỗng hoặc chỉ có 1 phần tử:** `sortCartBySubtotal`, `removeProductFromCart`, `clearCart` đều kiểm tra `head == nullptr` đầu tiên để tránh lỗi truy cập con trỏ null.
2. **Tính ổn định khi trùng thành tiền:** Hai sản phẩm có cùng `subtotal()` giữ nguyên thứ tự ban đầu sau khi sắp xếp.
3. **Đơn giá âm do dữ liệu lỗi (`discount > price`):** Trong `createCartNode`, nếu `unitPrice < 0` thì tự động chặn dưới về `0` để không làm âm tổng hóa đơn.
4. **Voucher lớn hơn tổng thanh toán:** Trong `viewCart`, nếu `rawTotal + vat - voucherDiscount < 0` thì tổng cộng thanh toán được đặt về `0đ`.
5. **Giải phóng bộ nhớ an toàn:** Gọi `clearCart` 2 lần liên tiếp không gây lỗi giải phóng trùng vùng nhớ (double free).

---

## 5. Kết quả kiểm thử
Chương trình được kiểm thử độc lập thông qua `src/tests/test_cart.cpp` với 8 kịch bản kiểm thử toàn diện (kết quả lưu tại `output/test_results/test_cart.log`):
* **Test 1:** Thao tác trên giỏ rỗng (xem, sắp xếp, tìm, xóa) $\rightarrow$ **PASS**.
* **Test 2:** Thêm và sắp xếp giỏ có 1 phần tử $\rightarrow$ **PASS**.
* **Test 3:** Sắp xếp 5 phần tử có cặp trùng thành tiền (`140.000đ`), xác minh đúng thứ tự tăng dần và tính ổn định (mã `101` vẫn đứng trước `103`) $\rightarrow$ **PASS**.
* **Test 4:** Tính toán hiển thị thuế VAT 8%, áp dụng voucher thường và voucher vượt tổng tiền $\rightarrow$ **PASS**.
* **Test 5:** Tìm kiếm và cộng dồn số lượng sản phẩm đã có trong giỏ $\rightarrow$ **PASS**.
* **Test 6:** Xóa nút đầu (`101`), nút giữa (`102`), nút cuối (`105`) và xử lý mã không tồn tại (`999`) $\rightarrow$ **PASS**.
* **Test 7:** Chặn dưới đơn giá âm khi dữ liệu đầu vào bị lỗi $\rightarrow$ **PASS**.
* **Test 8:** Xóa toàn bộ giỏ hàng (`clearCart`) 2 lần liên tiếp $\rightarrow$ **PASS**.