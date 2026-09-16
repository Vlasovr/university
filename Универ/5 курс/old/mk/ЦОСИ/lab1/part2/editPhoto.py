import cv2
import matplotlib.pyplot as plt

IMAGE_PATH = "autumn.jpg"


def clamp(v):
    if v < 0:
        return 0
    if v > 255:
        return 255
    return v


def linear_contrast_color(img, g_min=0, g_max=255):
    h, w, _ = img.shape
    out = img.copy()

    fmin = [255, 255, 255]
    fmax = [0, 0, 0]

    for y in range(h):
        for x in range(w):
            for c in range(3):
                v = int(img[y, x, c])
                if v < fmin[c]:
                    fmin[c] = v
                if v > fmax[c]:
                    fmax[c] = v

    a = [0, 0, 0]
    b = [0, 0, 0]
    for c in range(3):
        a[c] = (g_max - g_min) / (fmax[c] - fmin[c])
        b[c] = g_min - a[c] * fmin[c]

    for y in range(h):
        for x in range(w):
            for c in range(3):
                f = int(img[y, x, c])
                g = a[c] * f + b[c]
                out[y, x, c] = clamp(int(g))

    return out


def show_two_images(img1, img2):
    plt.figure(figsize=(10, 5))

    plt.subplot(1, 2, 1)
    plt.imshow(cv2.cvtColor(img1, cv2.COLOR_BGR2RGB))
    plt.title("Исходное изображение")
    plt.axis("off")

    plt.subplot(1, 2, 2)
    plt.imshow(cv2.cvtColor(img2, cv2.COLOR_BGR2RGB))
    plt.title("Линейное контрастирование")
    plt.axis("off")
    plt.show()


def main():
    img = cv2.imread(IMAGE_PATH)
    if img is None:
        print("Ошибка загрузки изображения")
        return

    result = linear_contrast_color(img)
    show_two_images(img, result)

    cv2.imwrite("contrast_out.png", result)
    print("Сохранено: contrast_out.png")


if __name__ == "__main__":
    main()
