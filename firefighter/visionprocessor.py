import cv2
import numpy as np

# loads an image (for debugging) or a video capture
#frame = cv2.imread('../resources/blue-0-0-255.png')
capture = cv2.VideoCapture(0)

blob_detector = cv2.SimpleBlobDetector_create()

    
while(1):
    # For demonstration
    # captures each frame
    _, frame = capture.read()
    
    # processes the image
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    lower_blue = np.array([110, 50, 50])
    upper_blue = np.array([130, 255, 255])
    
    mask = cv2.inRange(hsv, lower_blue, upper_blue)
    res = cv2.bitwise_and(frame, frame, mask = mask)

    keypoints = blob_detector.detect(res)

    im_with_keypoints = cv2.drawKeypoints(res, keypoints, np.array([]), (0, 0, 255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    
    # for debugging only
    cv2.imshow('frame', frame)
    cv2.imshow('mask', mask)
    cv2.imshow('res', res)
    cv2.imshow('blobs', im_with_keypoints)
    

    # TODO: Determine primary colour in image
    # TODO: Apply image thresholding method
    # TODO: Contour the image
    # TODO: Return instructions for robot
    
    k = cv2.waitKey(5) & 0xFF
    if k == 27:
        break

capture.release()
cv2.destroyAllWindows()
