--This is an example Lua (https://www.lua.org) script to give a general idea of how to build scripts
--Press F5 or click the Run button to execute it
--Type "emu." to show a list of all available API function

function printInfo()
  --Get the emulation state
  state = emu.getState()
  
  --Get the mouse's state (x, y, left, right, middle)
  mouseState = emu.getMouseState()  
  
  --Select colors based on whether the left button is held down
  if mouseState.left == true then
    buffer = emu.getScreenBuffer()
    for i = 1, #buffer do
      buffer[i] = buffer[i] & 0xFFFF
    end
    emu.setScreenBuffer(buffer)

    bgColor = 0x30FF6020
    fgColor = 0x304040FF
  else 
    bgColor = 0x302060FF
    fgColor = 0x30FF4040
  end
  
  --Draw some rectangles and print some text
  emu.drawRectangle(8, 8, 128, 24, bgColor, true, 1)
  emu.drawRectangle(8, 8, 128, 24, fgColor, false, 1)
  emu.drawString(12, 12, "Frame: " .. state["frameCount"], 0xFFFFFF, 0xFF000000)
  emu.drawString(12, 21, "Clock: " .. state["masterClock"], 0xFFFFFF, 0xFF000000)
  
  emu.drawRectangle(8, 218, 193, 11, bgColor, true, 1)
  emu.drawRectangle(8, 218, 193, 11, fgColor, false, 1)  
  emu.drawString(11, 220, "Hold left mouse button to switch colors", 0xFFFFFF, 0xFF000000)
  
  --Draw a block behind the mouse cursor - leaves a trail when moving the mouse
  emu.drawRectangle(mouseState.x - 2, mouseState.y - 2, 5, 5, 0xAF00FF90, true, 20)
  emu.drawRectangle(mouseState.x - 2, mouseState.y - 2, 5, 5, 0xAF000000, false, 20)
end

function drawSpriteHitbox(objDataStart)

  hitBoxOffsetX = emu.read(objDataStart + 12, emu.memType.snesWorkRam, false)
  hitBoxOffsetY = emu.read(objDataStart + 13, emu.memType.snesWorkRam, false)
  hitBoxSizeX = emu.read(objDataStart + 14, emu.memType.snesWorkRam, false)
  hitBoxSizeY = emu.read(objDataStart + 15, emu.memType.snesWorkRam, false)
  
  calHitBoxLeft = emu.read(objDataStart + 16, emu.memType.snesWorkRam, false)
  calHitBoxRight = emu.read(objDataStart + 17, emu.memType.snesWorkRam, false)
  calHitBoxTop = emu.read(objDataStart + 18, emu.memType.snesWorkRam, false)
  calHitBoxBottom = emu.read(objDataStart + 19, emu.memType.snesWorkRam, false)
  
  print(calHitBoxLeft)
  
  screenY = emu.read(objDataStart + 1, emu.memType.snesWorkRam, false)
  screenX = emu.read(objDataStart, emu.memType.snesWorkRam, false)
  
  emu.drawRectangle(screenX + hitBoxOffsetX, screenY + hitBoxOffsetY, hitBoxSizeX, hitBoxSizeY, 0x304040FF, false, 1) 
  emu.drawRectangle(calHitBoxLeft, calHitBoxTop, calHitBoxRight - calHitBoxLeft, calHitBoxBottom - calHitBoxTop, 0x30FF000F, false, 2) 

  emu.drawRectangle(8, 8, 128, 40, 0x304040FF, true, 1)
  emu.drawRectangle(8, 8, 128, 40, 0x304040FF, false, 1)
  if(objDataStart == 0x1fe3) then
	  emu.drawString(12, 12, "Player Left Pos: " .. calHitBoxLeft, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 21, "Player Right Pos: " .. calHitBoxRight, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 30, "Player Top Pos: " .. calHitBoxTop, 0xFFFFFF, 0xFF000000)
	  emu.drawString(12, 39, "Player Bottom Pos: " .. calHitBoxBottom, 0xFFFFFF, 0xFF000000)
  end
end

function drawAllHitboxes()
	-- player object
	drawSpriteHitbox(0x1fe3)
	-- target object
	drawSpriteHitbox(0x1fad)
end

--Register some code (printInfo function) that will be run at the end of each frame
-- emu.addEventCallback(printInfo, emu.eventType.endFrame);
emu.addEventCallback(drawAllHitboxes, emu.eventType.endFrame);

--Display a startup message
emu.displayMessage("Script", "Example Lua script loaded.")