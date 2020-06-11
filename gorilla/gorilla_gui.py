#!/usr/bin/python3
import os
import sys
import yaml
import pygame
import BackGround as bgd


class dict_to_obj(object):
    def __init__(self, adict):
        self.__dict__.update(adict)


class Listener:

    def __init__(self):
        self.fifo = '/tmp/duels_gorilla'
        if os.path.exists(self.fifo):
            os.remove(self.fifo)

        os.mkfifo(self.fifo)

    def read(self, display=False):
        with open(self.fifo) as fifo:
            data = yaml.load(fifo.read())
            if display:
                print('(Python) Just read: {}'.format(data))
                #print('Display is running!');
            return dict_to_obj(data)


if __name__ == '__main__':

    # main function
    listener = Listener()

    # init_msg has the same structure as C++ initMsg
    listener.__init__()
    init_msg = listener.read(True)

    # init display / structure from init_msg
    if 'refresh' in init_msg.__dict__:
        refresh = init_msg.__dict__['refresh']
    else:
        refresh = 100

    if 'init' in init_msg.__dict__:
        data_init = init_msg.__dict__['init']
        goPosx1 = data_init[2]['x1']
        goPosy1 = data_init[3]['y1']
        goPosx2 = data_init[4]['x2']
        goPosy2 = data_init[5]['y2']
        buildHeight = data_init[6]['yb']
        radius = data_init[7]['radius']
        gorPos = []
        gorPos.append(goPosx1)
        gorPos.append(goPosy1)
        gorPos.append(goPosx2)
        gorPos.append(goPosy2)

        winSurface = pygame.display.set_mode((bgd.SCR_WIDTH, bgd.SCR_HEIGHT), 0, 32)
        pygame.display.set_caption('Gorillas.py')
        winSurface.fill(bgd.SKY_COLOR)
        bgd.makeCityScape(winSurface,buildHeight)
        bgd.drawSun(winSurface)
        bgd.drawGorilla(winSurface,gorPos[0], gorPos[1])
        bgd.drawGorilla(winSurface, gorPos[2], gorPos[3])
        pygame.display.update()

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
    pygame.quit()
    sys.exit()




    winner = 0
    while winner == 0:
        # display_msg has the same structure as C++ displayMsg
        display_msg = listener.read(True)

        
        # update display from display_msg
        
        
        winner = display_msg.winner

    print('(Python) Player {} has won!'.format(winner))
