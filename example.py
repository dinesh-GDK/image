import os
import matplotlib.pyplot as plt
from PIL import Image

def main():
    BASE_DIR = "sample"
    IMAGE_NAME = "parrot"
    file_path = os.path.join(BASE_DIR, f"{IMAGE_NAME}.jpg")

    color_dith_path = os.path.join(BASE_DIR, f"{IMAGE_NAME}_dith.jpg")
    color_error_path = os.path.join(BASE_DIR, f"{IMAGE_NAME}_error.jpg")
    color_mbvq_path = os.path.join(BASE_DIR, f"{IMAGE_NAME}_mbvq.jpg")
    bw_dith_path = os.path.join(BASE_DIR, f"{IMAGE_NAME}_dith_bw.jpg")
    bw_error_path = os.path.join(BASE_DIR, f"{IMAGE_NAME}_mbvq_bw.jpg")

    os.system(f"./image_print --input={file_path} --output={color_dith_path} --op=1 --size=16")
    os.system(f"./image_print --input={file_path} --output={color_error_path} --op=2 --kernel=3 --threshold=127 --mbvq=0")
    os.system(f"./image_print --input={file_path} --output={color_mbvq_path} --op=2 --kernel=3 --threshold=127 --mbvq=1")
    os.system(f"./image_print --input={file_path} --output={bw_dith_path} --op=1 --size=16 --bw=1")
    os.system(f"./image_print --input={file_path} --output={bw_error_path} --op=2 --kernel=3 --threshold=127 --mbvq=0 --bw=1")

    all_paths = [file_path, color_dith_path, color_error_path, color_mbvq_path, bw_dith_path, bw_error_path]

    _, ax = plt.subplots(2, 3, figsize=(100, 100))
    for i in range(2):
        for j in range(3):
            ax[i, j].imshow(Image.open(all_paths[i*3+j]))
            ax[i, j].set_xlabel(f"({i*3+j+1})", fontsize=150)

    plt.tight_layout()
    plt.savefig("assests/demo_image.jpg", bbox_inches="tight")

if __name__ == "__main__":
    main()