import pygame
import sys
import time
pygame.init()

size = width, height = 1000, 1000

screen = pygame.display.set_mode(size)

RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

TOP_CUBE = pygame.Rect(450, 250, 100, 100)
MIDDLE_CUBE = pygame.Rect(450, 360, 100, 100)
BOTTOM_CUBE = pygame.Rect(450, 470, 100, 100)

TOP_COLOR = pygame.Color(0, 0, 255)
MIDDLE_COLOR = pygame.Color(0, 0, 255)
BOTTOM_COLOR = pygame.Color(0, 0, 255)

SENSOR_ONE = pygame.Rect(340, 600, 20, 20)
SENSOR_TWO = pygame.Rect(440, 650, 20, 20)
SENSOR_THREE = pygame.Rect(540, 650, 20, 20)
SENSOR_FOUR = pygame.Rect(640, 600, 20, 20)

sensors = [SENSOR_ONE, SENSOR_TWO, SENSOR_THREE, SENSOR_FOUR]

upslope = range(255)
downslope = list(reversed(upslope))
color_trails = [upslope, [0] * 255, downslope]
R=0
G=1
B=2

frames = [0, -200, -400]
targets = [0, -200, -400]

top_target = -400
middle_target = -200
bottom_target = 0

top_frame = -400
middle_frame = -200
bottom_frame = 0

def setup():
	pygame.draw.rect(screen, WHITE, SENSOR_ONE)
	pygame.draw.rect(screen, WHITE, SENSOR_TWO)
	pygame.draw.rect(screen, WHITE, SENSOR_THREE)
	pygame.draw.rect(screen, WHITE, SENSOR_FOUR)
	pygame.draw.rect(screen, TOP_COLOR, TOP_CUBE)
	pygame.draw.rect(screen, MIDDLE_COLOR, MIDDLE_CUBE)
	pygame.draw.rect(screen, BOTTOM_COLOR, BOTTOM_CUBE)

def redrawTrail(current_step):
	pygame.draw.rect(screen, BLACK, (100, 740, 255, 200))
	pygame.draw.line(screen, WHITE, (100 + current_step, 775), (100 + current_step, 925))
	for step, val in enumerate(color_trails[0]):
		pygame.draw.circle(screen, RED, (100 + step, 800 - val/10), 1)
	for step, val in enumerate(color_trails[1]):
		pygame.draw.circle(screen, GREEN, (100 + step, 850 - val/10), 1)
	for step, val in enumerate(color_trails[2]):
		pygame.draw.circle(screen, BLUE, (100 + step, 900 - val/10), 1)
	for step in range(255):
		pygame.draw.circle(screen, pygame.Color(color_trails[R][step], color_trails[G][step], color_trails[B][step]), (100 + step, 750), 1)

def moveFrames(frames, targets):
	frames[0] = frames[0] + (targets[0]-frames[0])/100
	return frames

setup()

FPS = 10
clock = pygame.time.Clock()

while True:
	milliseconds = clock.tick(FPS)
	print(milliseconds)
	for event in pygame.event.get():
		if event.type == pygame.QUIT: sys.exit()
		if event.type == pygame.MOUSEBUTTONUP:
			pos = pygame.mouse.get_pos()
			print pos
			for sensor in sensors:
				if sensor.collidepoint(pos):
					print('triggered')
					targets[0] += 200
	pygame.draw.rect(screen, pygame.Color(color_trails[R][frames[0]], color_trails[G][frames[0]], color_trails[B][frames[0]]), BOTTOM_CUBE)
	frames = moveFrames(frames, targets)
	print frames
	pygame.display.flip()
	redrawTrail(frames[0])