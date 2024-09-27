--======================================================================
--  A simple clock in love
--
--  This program is released under MIT license
--
--  Dov Grobgeld
--  2024-09-26 Thu
------------------------------------------------------------------------


local font
local timeString = ""
local evenSecond = true

function love.load()
    font = love.graphics.newFont("assets/Roboto-Bold.ttf", 240)
    love.graphics.setFont(font)
end

function love.update(dt)
    local time = os.date("*t")
    timeString = string.format("%02d:%02d", time.hour, time.min)
    evenSecond = time.sec % 2 == 0
end

function love.draw()
    love.graphics.clear(0, 0, 0) -- Clear the screen with black color
    local width = love.graphics.getWidth()
    local height = love.graphics.getHeight()
    local textWidth = font:getWidth(timeString)
    local textHeight = font:getHeight()
    local x = (width - textWidth) / 2
    local y = (height - textHeight) / 2

    -- Function to draw text with a halo effect
    local function drawHaloText(text, x, y)
        local haloColor = {1, 0, 0, 0.5} -- Red color with transparency for halo
        local textColor = {1, 1, 0} -- Yellow color for text
        local offset = 5 -- Offset for the halo

        -- Draw halo
        love.graphics.setColor(haloColor)
        for dx = -offset, offset do
            for dy = -offset, offset do
                if dx ~= 0 or dy ~= 0 then
                    love.graphics.print(text, x + dx, y + dy)
                end
            end
        end

        -- Draw text
        love.graphics.setColor(textColor)
        love.graphics.print(text, x, y)
    end

    -- Draw the hours and minutes with halo
    drawHaloText(string.sub(timeString, 1, 2), x, y)
    drawHaloText(string.sub(timeString, 4, 5), x + font:getWidth("00:"), y)
    
    -- Draw the colon with halo
    if evenSecond then
        drawHaloText(":", x + font:getWidth("00"), y)
    else
        love.graphics.setColor(0, 0, 0) -- Black color for odd seconds
        love.graphics.print(":", x + font:getWidth("00"), y)
    end
end

function love.joystickpressed(joystick, button)
    if button == 1 then
        love.event.quit()
    end
    if button == 2 then
        triggerTime = love.timer.getTime()
    end
end
