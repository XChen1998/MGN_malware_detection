class Colors: 
    """
    color output class, provides escape codes for coloring
    use case :
    Any of these strings
    """
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    END = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    colorsArray = [HEADER, OKGREEN, OKBLUE, WARNING, FAIL, END, BOLD, UNDERLINE]
