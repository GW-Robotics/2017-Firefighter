import cv2
import numpy as np

# loads an image (for debugging) or a video capture
capture = cv2.VideoCapture(0)

kernel = np.ones((5,5), np.uint8)

def filter_blue(img):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    lower_blue = np.array([110, 50, 50])
    upper_blue = np.array([130, 255, 255])
    
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    res = cv2.bitwise_and(frame, frame, mask = mask)

    return res

def auto_canny(img):
    v = np.median(img)
	
    lower = int(max(0, (1.0 - 0.33) * v))
    upper = int(min(255, (1.0 + 0.33) * v))

    return cv2.Canny(img, lower, upper)

def contour_rectangles(img):
    _, contours, _ = cv2.findContours(img.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key = cv2.contourArea, reverse = True)[:40]
    rectangles = []

    for c in contours:
        peri = cv2.arcLength(c, True)
        approx = cv2.approxPolyDP(c, 0.02 * peri, True)

        if len(approx) in range(4,6):
            rectangles.append(c)

    return rectangles

while(1):
    # For demonstration
    # captures each frame
    _, frame = capture.read()
    #_, frame_full = capture.read()

    #frame = cv2.resize(frame_full, (0,0), fx=0.5, fy=0.5)
    
    # processes the image
    res = filter_blue(frame)

    greyscale = cv2.cvtColor(res, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(greyscale, (3, 3), 0)

    # img_dilation = cv2.dilate(blurred, kernel, iterations=1)
	
    edged = auto_canny(blurred)
    rectangles = contour_rectangles(edged)

    cv2.drawContours(frame, rectangles, -1, (255, 0, 0), 3)
    
    # for debugging only
    cv2.imshow('frame', frame)
    # cv2.imshow('mask', mask)
    # cv2.imshow('res', res)
    # cv2.imshow('edgy', edged)
    
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

capture.release()
cv2.destroyAllWindows()
