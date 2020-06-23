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
        with open(self.fifo, 'r') as fifo :
            data = yaml.load(fifo.read()) or {}

        if display:
            print('(Python) Just read: {}'.format(data))
            print('Display is running!')
        return dict_to_obj(data)

class InitMsg:

    def __init__(self, listener):
        self.listen = listener

    def __enter__(self):
        self.ob = self.listen.read(False)
        self.data = self.ob.__dict__
        return self.ob

    def __exit__(self, exc_type, exc_val, exc_tb):
        if 'init' not in self.data:
            print("type: ", exc_type)
            print("value: ", exc_val)
            print("trace: ", exc_tb)

if __name__ == '__main__':

    # main function
    listener = Listener()
    listener.__init__()
    refresh = 100
    # init_msg has the same structure as C++ initMsg
    #init_msg = listener.read(True)

    # init display / structure from init_msg
    #if 'init' in init_msg.__dict__:
    #    data_init = init_msg.__dict__['init']
    with InitMsg(listener) as init_msg:
        data_init = init_msg.init

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
    p1 = "player1"
    p2 = "player2"

    #initialise pygame
    pygame.init()

    winSurface = pygame.display.set_mode((bgd.SCR_WIDTH, bgd.SCR_HEIGHT), 0, 32)
    pygame.display.set_caption('Gorillas.py')
    skylineSurf = bgd.makeCityScape(buildHeight)
    bgd.drawSun(skylineSurf)
    bgd.drawGorilla(skylineSurf, gorPos[0], gorPos[1])
    bgd.drawGorilla(skylineSurf, gorPos[2], gorPos[3])
    bgd.drawText(p1, skylineSurf, 2, 2, bgd.WHITE_COLOR, bgd.SKY_COLOR)
    bgd.drawText(p2, skylineSurf, 538, 2, bgd.WHITE_COLOR, bgd.SKY_COLOR)
    winSurface.blit(skylineSurf, (0, 0))
    pygame.display.update()



    winner = 0
    orient = 1
    while winner == 0:
        # display_msg has the same structure as C++ displayMsg
        display_msg = listener.read()
        # exit if received a exit msg
        # update display from display_msg
        ban_x = display_msg.x
        ban_y = display_msg.y
        hit = display_msg.hit
        winner = display_msg.winner
        wind = display_msg.wind

        bgd.drawText(str(wind), winSurface, bgd.SCR_WIDTH / 2, 335, bgd.WHITE_COLOR, bgd.SKY_COLOR,
                     fontSize=15, pos='center')
        pygame.display.update()
        if ban_x < 0 or ban_x > bgd.SCR_WIDTH:
            continue
        else:
            if hit == 0:
                bgd.displayBanana(winSurface, orient, ban_x, ban_y)
                if orient >= 3:
                    orient = 0
                else:
                    orient += 1
            else:
                if winner == 1 or winner == 2:
                    '''Do explosion and game over'''
                    pygame.draw.circle(skylineSurf, bgd.SKY_COLOR, (ban_x, ban_y), radius+10)
                    winSurface.blit(skylineSurf, (0, 0))
                    endword = "Playerv {} won the game".format(winner)
                    print('(Python) Player {} has won!'.format(winner))
                else:
                    '''Do explosion'''
                    pygame.draw.circle(skylineSurf, bgd.SKY_COLOR, (ban_x, ban_y), radius)
                    endword = "it is a draw"

        winSurface.blit(skylineSurf, (0, 0))
        pygame.display.update()
        # assure can exit the game at anytime
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

        # display_msg has the same structure as C++ displayMsg
        # update display from display_msg

    """show the gameover screen"""
    pygame.time.wait(500)
    winSurface.fill(bgd.BLACK_COLOR)
    bgd.drawText('G A M E   O V E R', winSurface, bgd.SCR_WIDTH / 2, 15, bgd.WHITE_COLOR, bgd.BLACK_COLOR, fontSize=40, pos='center')
    bgd.drawText(endword, winSurface, bgd.SCR_WIDTH / 2, 110, bgd.WHITE_COLOR, bgd.BLACK_COLOR, pos='center')
    pygame.display.update()
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
