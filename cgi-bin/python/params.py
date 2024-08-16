#!/usr/bin/env python3

# eg call with http://localhost:8080/params.py?name1=value1&name2=value2

import os
import urllib.parse

def main():
    # Get the query string and PATH_INFO from environment variables
    query_string = os.getenv("QUERY_STRING", "")
    path_info = os.getenv("PATH_INFO", "")

    # Parse the query string into key-value pairs
    form = urllib.parse.parse_qs(query_string)    

    # Display the PATH_INFO
    print("<h2>PATH_INFO</h2>")
    print("<p>{}</p>".format(path_info))

    # Display the Query String parameters
    print("<h2>Query String Parameters</h2>")
    if form:
        print("<ul>")
        for key, values in form.items():
            for value in values:
                print("<li><strong>{}</strong>: {}</li>".format(key, value))
        print("</ul>")
    else:
        print("<p>No query string parameters found.</p>")


if __name__ == "__main__":
    main()
