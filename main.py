import argparse
import os
import shutil
from PIL import Image
from PIL.ExifTags import TAGS


# Function to get year from EXIF-data of the image
def get_image_year(image_path):
    try:
        image = Image.open(image_path)
        exif_data = image.getexif()
        if exif_data is not None:
            year = exif_data[306][:4]
            return year
    except Exception as e:
        print(f"Cannot get data for {image_path}: {e}")
    return None


def move_image_by_year(image_path, destination_folder):
    year = get_image_year(image_path)
    if year:
        year_folder = os.path.join(destination_folder, year)
        if not os.path.exists(year_folder):
            os.makedirs(year_folder, exist_ok=True)
        shutil.move(image_path, os.path.join(year_folder, os.path.basename(image_path)))
        print(f"The File {image_path} was moved to {year_folder}")
    else:
        print(f"Date was not found {image_path}, the file was not moved.")


def organize_photos_by_year(source_folder, destination_folder):
    for root, dirs, files in os.walk(source_folder):
        for file in files:
            file_path = os.path.join(root, file)
            if file.lower().endswith(('.jpg', '.jpeg', '.png', '.tiff', '.bmp', '.gif')):
                move_image_by_year(file_path, destination_folder)


def main():
    parser = argparse.ArgumentParser(description="Move photos by year based on creation date")

    parser.add_argument('source_folder', type=str, help='Path to the source folder with photos')
    parser.add_argument('destination_folder', type=str, help='Folder path for moving photos by year')

    args = parser.parse_args()

    if not os.path.exists(args.source_folder):
        print(f"Source folder not found: {args.source_folder}")
    else:
        organize_photos_by_year(args.source_folder, args.destination_folder)
        print("Done!")


if __name__ == "__main__":
    main()
