local ui_scripts = {
    "widget_library",
    "widget_statistics",
    "widget_demo",
}

for i=1, #ui_scripts do
    require('ui.' .. ui_scripts[i])
end