--======================================================================
--  A simple colorful clock in Löve
--
--  This program is released under MIT license
--
--  Dov Grobgeld
--  2024-09-26 Thu
------------------------------------------------------------------------

-- Load the LÖVE framework
function love.load()
    -- The directory containing the glyph images
    local glyphDir = "image-font/"
    
    -- Table to store the loaded glyph images
    glyphs = {}
    
    -- Get all files in the glyph directory
    local files = love.filesystem.getDirectoryItems(glyphDir)
    
    -- Load each glyph image into the glyphs table
    for _, file in ipairs(files) do
        local glyphId = file:match("glyph(%x%x)%.png")
        if glyphId then
            local imagePath = glyphDir .. file
            glyphs[glyphId] = love.graphics.newImage(imagePath)
        end
    end
end

function get_char_width(char)
    local glyphId = string.format("%02X", string.byte(char))
    local glyph = glyphs[glyphId]
    local placeholderGlyph = glyphs["30"] -- Use the width of '0' as a placeholder
    local placeholder_width  = placeholderGlyph:getWidth()

    if glyph then
        local width = glyph:getWidth()
        if char == ":" or char == " " then
            -- Handle the colon and space characters
            return placeholder_width / 2
        else
            return width
        end
    else
        -- Handle the colon and space characters
        return placeholder_width / 2
    end
end

-- Function to write a string at a given position (x, y)
function write_string(x, y, str)
    local startX = x
    for i = 1, #str do
        local char = str:sub(i, i)
        local glyphId = string.format("%02X", string.byte(char))
        local glyph = glyphs[glyphId]
        local placeholderGlyph = glyphs["30"] -- Use the width of '0' as a placeholder
        local placeholder_width  = placeholderGlyph:getWidth()
        local char_width = get_char_width(char)

        if glyph then
            local width = glyph:getWidth()
            if char == ":" then
              shiftx=15
            else
              shiftx=0
            end
            love.graphics.draw(glyph, x+shiftx, y)
        end
        x = x + char_width
    end
end

-- Function to calculate the width of a string in pixels
function string_width(str)
    local width = 0
    for i = 1, #str do
        local char = str:sub(i, i)
        local glyphId = string.format("%02X", string.byte(char))
        local glyph = glyphs[glyphId]
        width = width + get_char_width(char)
    end
    return width
end

-- Function to write a string centered in the x direction
function write_string_center(y, str)
    local screenWidth = love.graphics.getWidth()
    local strWidth = string_width(str)
    local x = (screenWidth - strWidth) / 2
    write_string(x, y, str)
end

-- Draw function to demonstrate the usage
function love.draw()
    local y = love.graphics.getHeight() / 2 - 80
    local time = os.date("*t")
    local timeStr

    if time.sec % 2 == 0 then
        timeStr = string.format("%02d:%02d", time.hour, time.min)
    else
        timeStr = string.format("%02d %02d", time.hour, time.min)
    end

    write_string_center(y, timeStr)
end

function love.joystickpressed(joystick, button)
    if button == 1 then
        love.event.quit()
    end
    if button == 2 then
        triggerTime = love.timer.getTime()
    end
end
