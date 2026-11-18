# EGM Issue Downloader from EGM compendium

Some script I used to download EGM, EGM2, and buyers guide from the EGM
compendium so I could sideload onto my tablet. Script has several large delays
in it so that the script doesn't pound their website too hard.  So it will
take a few hours to complete.

# Script requires login cookie

Since the compendium is only available to paid subscribers, you need to prove
that you have a valid account.

1. Login to EGM compendium website with chrome
2. Extract cookies. DevTools -> Application -> Cookies -> Highlight all rows
   and cols -> Ctrl-C -> Paste into an empty text file
3. Convert cookies to netscape cookie format:

```
convert_chrome_cookie_to_netscape.py chrome_cookies.txt > netscape_cookies.txt
```

4. Run this script

Download single issue (use 3 digit issue numbers for EGM and EGM2, 4 digit
year for the buyers guide):

```
./download_egm.sh 001
./download_egm2.sh 001
./download_buyers_guide.sh 1992
```

Download range of issues:

```
./download_egm.sh 001 295
```


