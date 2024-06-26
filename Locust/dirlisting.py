import os

def dir_list(dir_str, url, real_dir):
    try:
        files = os.listdir(dir_str)
    except OSError as e:
        return f"Error: {e}"

    html = "<!DOCTYPE html>\n"
    html += "<html><head></head><body>"
    html += f"<h1>Directory listing for {real_dir}</h1><hr>"
    html += "<ul>"

    new_url = url + "/"

    for file_name in files:
        if file_name not in [".", ".."]:
            file_path = os.path.join(dir_str, file_name)
            if not check_if_deleted(file_path):
                html += f'<li><a href="/{new_url}{file_name}">{file_name}</a></li>'

    html += "</ul><hr></body></html>"
    
    return html

def check_if_deleted(file_path):
    # Placeholder function to simulate the Overseer's checkIfDeleted method
    # Implement the actual logic as required
    return False



if __name__ == "__main__":
    print (dir_list("/home/codespace/recovery/turnInPage/files", "files", "./turnInPage/files/"))