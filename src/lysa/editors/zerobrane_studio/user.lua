--[[--
  Lysa ZeroBrane user preferences example
--]]--
editor.autoactivate = true
editor.fontname = "Fira Code"
editor.fontsize = 12
editor.tabwidth = 4
editor.usetabs = true
editor.backspaceunindent = true

console.fontname = "Fira Code"
console.fontsize = 12

output.fontname = "Fira Code"
output.fontsize = 12

filetree.fontname = "Fira Code"
filetree.fontsize = 10

debugger.runonstart = false

api = {
    'baselib',
    'lysa'
}
autocomplete = true

styles = loadfile('cfg/tomorrow.lua')('SolarizedLight')
stylesoutshell = styles
styles.auxwindow = loadfile('cfg/tomorrow.lua')('SolarizedDark').text