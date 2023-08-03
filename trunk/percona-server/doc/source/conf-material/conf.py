#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys
import os
sys.path.append(os.path.abspath("../"))
from conf import *
extensions.append('sphinx_gitstamp')
extensions.append('sphinx_copybutton')
html_sidebars['**']=['globaltoc.html', 'searchbox.html', 'localtoc.html', 'logo-text.html']
html_theme = 'sphinx_material'
html_theme_options = {
    'base_url': 'http://bashtage.github.io/sphinx-material/',
    'repo_url': 'https://github.com/percona/percona-server',
    'repo_name': 'percona/percona-server',
    'color_accent': 'grey',
    'color_primary': 'orange',
    'globaltoc_collapse': True,
    'version_dropdown': True,
    'version_dropdown_text': 'Versions',
    'version_info': {
        "5.7": "https://docs.percona.com/percona-server/5.7/",
        "8.0": "https://docs.percona.com/percona-server/8.0/",
        "Latest": "https://docs.percona.com/percona-server/latest/"
    },
}
html_logo = '../_images/percona-logo.svg'
html_favicon = '../_images/percona_favicon.ico'
pygments_style = 'emacs'
gitstamp_fmt = "%b %d, %Y"
# Specify the text pattern that won't be copied with the code block contents
copybutton_prompt_text = '$'
# Add any paths that contain templates here, relative to this directory.
templates_path = ['../_templates/theme']
#html_last_updated_fmt = ''

