--[[--
  Lysa ZeroBrane user preferences example
--]]--
editor.autoactivate = true
editor.fontname = "Fira Code"
editor.fontsize = 11
editor.tabwidth = 4
editor.usetabs = true
editor.backspaceunindent = true

console.fontname = "Fira Code"
console.fontsize = 11

output.fontname = "Fira Code"
output.fontsize = 11

filetree.fontname = "Fira Code"
filetree.fontsize = 11

debugger.runonstart = false

api = {
    'baselib',
    'lysa',
    'vireo'
}
autocomplete = true

styles = loadfile('cfg/tomorrow.lua')('SolarizedLight')
stylesoutshell = styles
styles.auxwindow = loadfile('cfg/tomorrow.lua')('SolarizedDark').text