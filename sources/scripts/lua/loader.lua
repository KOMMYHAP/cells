local scripts = {
    "widget_library",
    "widget_statistics",
    "widget_demo",
}

for i=1, #scripts do
    require(scripts[i])
end