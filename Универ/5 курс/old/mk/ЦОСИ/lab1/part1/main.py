import cv2
import matplotlib.pyplot as plt


COLOR_PATH = "pexels-photo-210019.jpeg"
GRAY_PATH = "cameraman.tif"


def to_uint8(img):
    if img.dtype != "uint8":
        img = cv2.convertScaleAbs(img)
    return img


def manual_brightness_histogram(img):
    hist = [0] * 256

    if len(img.shape) == 2:
        h, w = img.shape
        for y in range(h):
            for x in range(w):
                hist[int(img[y, x])] += 1
        return hist

    h, w, _ = img.shape
    for y in range(h):
        for x in range(w):
            b, g, r = img[y, x]
            Y = int(0.114 * b + 0.587 * g + 0.299 * r)
            if Y < 0:
                Y = 0
            if Y > 255:
                Y = 255
            hist[Y] += 1
    return hist


def negative(img):
    if len(img.shape) == 2:
        h, w = img.shape
        out = img.copy()
        for y in range(h):
            for x in range(w):
                out[y, x] = 255 - int(img[y, x])
        return out

    h, w, _ = img.shape
    out = img.copy()
    for y in range(h):
        for x in range(w):
            b, g, r = img[y, x]
            out[y, x] = (255 - b, 255 - g, 255 - r)
    return out


def high_pass_filter_3x3(img):
    k = [
        [-1, -1, -1],
        [-1,  8, -1],
        [-1, -1, -1],
    ]

    if len(img.shape) == 2:
        h, w = img.shape
        out = img.copy()
        for y in range(1, h - 1):
            for x in range(1, w - 1):
                s = 0
                for dy in range(-1, 2):
                    for dx in range(-1, 2):
                        s += int(img[y + dy, x + dx]) * k[dy + 1][dx + 1]
                s = 0 if s < 0 else (255 if s > 255 else s)
                out[y, x] = s
        return out

    h, w, _ = img.shape
    out = img.copy()
    for y in range(1, h - 1):
        for x in range(1, w - 1):
            for c in range(3):  # B,G,R
                s = 0
                for dy in range(-1, 2):
                    for dx in range(-1, 2):
                        s += int(img[y + dy, x + dx, c]) * k[dy + 1][dx + 1]
                s = 0 if s < 0 else (255 if s > 255 else s)
                out[y, x, c] = s
    return out


def prepare_all(img):
    img = to_uint8(img)

    if len(img.shape) == 3 and img.shape[2] == 4:
        img = cv2.cvtColor(img, cv2.COLOR_BGRA2BGR)

    img_neg = negative(img)
    img_hpf = high_pass_filter_3x3(img)

    h0 = manual_brightness_histogram(img)
    h1 = manual_brightness_histogram(img_neg)
    h2 = manual_brightness_histogram(img_hpf)

    return (img, img_neg, img_hpf), (h0, h1, h2)


def ax_show_image(ax, img, title):
    ax.set_title(title)
    ax.axis("off")
    if len(img.shape) == 2:
        ax.imshow(img, cmap="gray")
    else:
        ax.imshow(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))


def ax_show_hist(ax, hist, title):
    ax.set_title(title, fontsize=10)
    ax.bar(range(256), hist)

    ax.set_xlabel("Яркость (0..255)", fontsize=9)
    ax.set_ylabel("Количество пикселей", fontsize=9)

    ax.tick_params(axis='both', labelsize=8)


def main():
    img_color = cv2.imread(COLOR_PATH, cv2.IMREAD_UNCHANGED)
    img_gray = cv2.imread(GRAY_PATH, cv2.IMREAD_UNCHANGED)

    if img_color is None:
        print("Не удалось открыть цветное изображение:", COLOR_PATH)
        return
    if img_gray is None:
        print("Не удалось открыть полутоновое изображение:", GRAY_PATH)
        return

    if len(img_gray.shape) == 3:
        img_gray = cv2.cvtColor(img_gray, cv2.COLOR_BGR2GRAY)

    (c0, c1, c2), (hc0, hc1, hc2) = prepare_all(img_color)
    (g0, g1, g2), (hg0, hg1, hg2) = prepare_all(img_gray)

    cv2.imwrite("color_negative.png", c1)
    cv2.imwrite("color_hpf.png", c2)
    cv2.imwrite("gray_negative.png", g1)
    cv2.imwrite("gray_hpf.png", g2)


    fig, axes = plt.subplots(
        3, 4,
        figsize=(22, 15),
        constrained_layout=True
    )

    ax_show_image(axes[0, 0], c0, "Цветное: исходное")
    ax_show_hist(axes[0, 1], hc0, "Яркость: исходное (цветное)")
    ax_show_image(axes[0, 2], g0, "Полутоновое: исходное")
    ax_show_hist(axes[0, 3], hg0, "Яркость: исходное (полутоновое)")

    ax_show_image(axes[1, 0], c1, "Цветное: негатив")
    ax_show_hist(axes[1, 1], hc1, "Яркость: негатив (цветное)")
    ax_show_image(axes[1, 2], g1, "Полутоновое: негатив")
    ax_show_hist(axes[1, 3], hg1, "Яркость: негатив (полутоновое)")

    ax_show_image(axes[2, 0], c2, "Цветное: HPF")
    ax_show_hist(axes[2, 1], hc2, "Яркость: HPF (цветное)")
    ax_show_image(axes[2, 2], g2, "Полутоновое: HPF")
    ax_show_hist(axes[2, 3], hg2, "Яркость: HPF (полутоновое)")

    plt.show()


if __name__ == "__main__":
    main()
