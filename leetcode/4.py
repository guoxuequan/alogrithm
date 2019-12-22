import sys
class Solution:
    def findMedianSortedArrays(self, nums1: List[int], nums2: List[int]) -> float:
        maxint = sys.maxsize
        minint = -sys.maxsize-1
        n = len(nums1)
        m = len(nums2)
        if n > m:
            Solution.findMedianSortedArrays(self,nums2,nums1)
        lo = 0
        hi = 2 * n
        while lo <= hi:
            c1 = (lo + hi) / 2
            c2 = m + n - c1
            
            LMax1 = minint if c1 == 0 else nums1[int((c1-1)/2)]
            RMin1 = maxint if c1 == 2 * n else nums1[int(c1/2)]
            LMax2 = minint if c2 == 0 else nums2[int((c2-1)/2)]
            RMin2 = maxint if c2 == 2 * m else nums2[int(c2/2)]
            
            if LMax1 > RMin2:
                hi = c1 - 1
            elif LMax2 > RMin1:
                lo = c1 + 1
            else:
                break
        return (max(LMax1, LMax2) + min(RMin1, RMin2)) / 2.0
