--[[
echo '' > code.lua && vim code.lua
]]

function read_at(cmd)
    io.write(cmd.."\r\n")
    lines = ""
    line=""
    while not (string.find(line, "OK") or string.find(line, "ERROR")) do
        line = io.read()
        if (line ~= nil and line ~= "" and line ~= "\r") then
            lines = lines.." "..line
        end
    end
    lines = string.gsub(lines, "\r", "")
    return lines
end


console_dev="/dev/ttyUSB4" -- workaround
os.execute("stty -F "..console_dev.." raw -echo -echoe -echok")
dev_file = io.open(console_dev, "a+")

io.input(dev_file)
io.output(dev_file)


--[[
Expected: ati Quectel EM06 Revision: EM06ELAR03A08M4G OK
]]

if #arg == 1 then
    if arg[1] == "1" then
        cmd="ATI"
    elseif arg[1] == "2" then
        cmd='AT+QCFG="band"?'
    end
end

val1 = read_at(cmd)
print(val1)

io.close(dev_file)
