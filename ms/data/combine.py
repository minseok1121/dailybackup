import os

# 출력 파일 이름 및 경로
output_file = "merged_output.txt"

# 현재 디렉토리에서 .txt 확장자를 가진 모든 파일 찾기
text_files = [f for f in os.listdir() if os.path.isfile(f) and f.endswith(".txt")]

# 파일을 하나의 큰 파일로 병합
with open(output_file, "w") as merged_file:
    for text_file in text_files:
        with open(text_file, "r") as current_file:
            merged_file.write(current_file.read())