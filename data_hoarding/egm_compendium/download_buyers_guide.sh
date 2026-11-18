#!/bin/bash

# Just like download EGM script except we have to change the link a little bit that we download from

#set_file_extension(filepath, file_ext) 
set_file_extension() {
	filepath=$1
	file_ext=$2

	cur_basename=$(basename $filepath)
	ext_strip_basename=$(basename -s $file_ext $filepath)

	echo "Cur basename = $cur_basename"
	echo "Strip basename = $ext_strip_basename"

	if [[ $cur_basename == $ext_strip_basename ]]; then
		echo "Adding $file_ext to $filepath"
		mv $filepath ${filepath}${file_ext}
	else
		echo "File $filepath already has the correct extension"
	fi
}


#def detect_and_set_file_ext(dir_name) {
detect_and_set_file_ext() {
	for x in $(find $1 -type f -exec echo {} \;); do
		echo $x
		is_zip_file=$(file $x | grep "Zip archive data" | wc -l)
		is_html_file=$(file $x | grep "HTML document" | wc -l)
		
		if [ $is_zip_file -eq "1" ]; then
			echo "$x is a zip file"
			set_file_extension $x .zip
		fi

		if [ $is_html_file -eq "1" ]; then
			echo "$x is a html file"
			set_file_extension $x .html
		fi

	done
}


# wget ocommand didn't work
#wget --load-cookies netscape_cookies.txt --keep-session-cookies --content-disposition -A "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/157.0.0.0 Safari/537.36" "https://egmcompendium.com/publications/egm/egm-031"

# the urls from compendium have a refresh arg, just ignore it

# download_link(output_path, url_path) 
download_link() {
	output_path=$1
	url_path=$2
	curl -L -J -O -b netscape_cookies.txt \
	-A "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/157.0.0.0 Safari/537.36" \
	-e "$url_path" \
	"$url_path" --output-dir $output_path
}

# download_single_issue(issue)
download_single_issue() {

	# issue should be something like 1999-video-game-buyers-guide
	issue=$1
	echo "Download link page for $issue"

	if [ ! -d $issue ]; then
		mkdir $issue
	fi

	issue_index_path="https://egmcompendium.com/publications/buyers-guide/$issue/"

	download_link $issue $issue_index_path

	echo "Giving the site a break"
	sleep 10

	while IFS= read -r line; do
		echo "Link found: $line"
		download_link $issue $line
		sleep 25
	done < <(cat $issue/* | grep -oE 'https.*wpdmdl=[0-9]*' | sort | uniq)

	detect_and_set_file_ext $issue
}

# download_multiple_issues(start, stop)
download_multiple_issues() {

	start_num=$1
	stop_num=$2
	for n in $(seq -f "%04g" $start_num $stop_num); do
		echo "Starting download of issue $n"
	download_single_issue $n-video-game-buyers-guide
	sleep 30
	done
}

check_cookie_file() {
	if [ ! -f netscape_cookies.txt ]; then
		echo "You need to create a cookie file!"
		echo "netscape_cookies.txt not found"
		exit 2
	fi
}




if [ $# -eq "1" ]; then
	echo "Single Issue Download"
	check_cookie_file
	download_single_issue $1-video-game-buyers-guide
	exit 0
fi

if [ $# -eq "2" ]; then
	echo "Range Issue Download"
	check_cookie_file
	download_multiple_issues $1 $2
	exit 0
fi

echo "EGM Issue Downloader from EGM compendium"
echo "Step 1 - Login to EGM compendium website with chrome"
echo "Step 2 - Extract cookies. DevTools -> Application -> Cookies -> Highlight all rows and cols -> Ctrl-C -> Paste in text file"
echo "Step 3 - Convert cookies to netscape cookie format: ./chrome2netscape.py chrome_cookies.txt > netscape_cookies.txt"
echo "Step 4 - Run this script"

echo "Ways to run the script:"
echo "Download single issue: ./download_issues.sh 001"
echo "Download range of issues: ./download_issues.sh 001 295"



