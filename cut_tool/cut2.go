/*******************************************************
* @author      : dog head
* @modified    : 2022-07-25 11:09
* @mail        : 396139919@qq.com
* @file        : cat.go
* @idea        : vim
* @description : TODO
*******************************************************/

package main

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"os"
	"os/exec"
	"strconv"
	"strings"
)

// 时间单位
const (
	// 定义每分钟的秒数
	SecondsPerMinute = 60
	// 定义每小时的秒数
	SecondsPerHour = SecondsPerMinute * 60
	// 定义每天的秒数
	SecondsPerDay = SecondsPerHour * 24
)
const (
	VideoFlag = "video"
	H264Flag  = "h264"
)

type Filter2EvlInfo struct {
	TotalTime  uint64    // 视频总时长
	ScoreCount int       // 计算次数
	Interval   uint64    // 每次计算时长
	CutTimeArr []CutTime // 裁剪信息
}

type CutTime struct {
	StartTime     uint64 // 计算开始时间数组
	VocalsPercent uint64 // 人声占比数组
	SegmentName   string // 分段文件名数组
}
type streamInfo struct {
	Index              int    `json:"index"`
	CodecName          string `json:"codec_name"`
	Profile            string `json:"profile"`
	CodecType          string `json:"codec_type"`
	Width              int    `json:"width"`
	Height             int    `json:"height"`
	CodedWidth         int    `json:"coded_width"`
	CodedHeight        int    `json:"coded_height"`
	HasBFrames         int    `json:"has_b_frames"`
	DisplayAspectRatio string `json:"display_aspect_ratio"`
	BitRate            string `json:"bit_rate"`
	BitsPerRawSample   string `json:"bits_per_raw_sample"`
	Nbframes           string `json:"nb_frames"`
}

type formatInfo struct {
	Filename   string `json:"filename"`
	NbStreams  int    `json:"nb_streams"`
	NbPrograms int    `json:"nb_programs"`
	FormatName string `json:"format_name"`
	Duration   string `json:"duration"`
	Size       string `json:"size"`
	BitRate    string `json:"bit_rate"`
	ProbeScore int    `json:"probe_score"`
}

type MediaInfo struct {
	Streams []streamInfo `json:"streams"`
	Format  formatInfo   `json:"format"`
}

func ffprobeMediaFile(traceId string, Url string) (MediaInfo, error) {
	var (
		out    bytes.Buffer
		result MediaInfo
	)

	cmdArguments := []string{"-show_format", "-show_streams", "-print_format", "json"}
	// Url = "\"" + Url + "\""
	arguments := append(cmdArguments, Url)

	cmd := exec.Command("ffprobe", arguments...)

	cmd.Stdout = &out
	err := cmd.Run()
	if err != nil {
		return result, err
	}

	err = json.Unmarshal(out.Bytes(), &result)
	if err != nil {
		return result, err
	}
	fmt.Println(out.String())

	return result, nil
}

func Filter2EvaluateInfo(mInfo MediaInfo) (*Filter2EvlInfo, error) {
	// 逻辑实现的简介: 默认选取 30s的时间进行评分，限制最大4段评分。
	//				 1.随着视频时长增大，取点间隔越大；
	//				 2.当视频不满30时，取消评分；
	//				 3.当视频不满10分钟时，取中间30s；
	info := Filter2EvlInfo{}
	// 1. interval time
	info.Interval = 15 // keep in 1m ~ 30s
	if info.Interval > 60 || info.Interval < 15 {
		return nil, errors.New("err interval time")
	}

	// 2. count ScoreTimes
	duration, _ := strconv.ParseFloat(mInfo.Format.Duration, 64)

	if duration > 2400 { // 时长大于4段10分钟的总时长，意味着到达分段限制
		info.ScoreCount = 12
	} else if duration > 600 && duration < 2400 { // 时长小于4段10分钟的总时长并大于10分钟，分段在2-3之间
		info.ScoreCount = int(duration/600) + 1
	} else { // 小于10分钟时长，分段为 1
		info.ScoreCount = 1
	}

	// 3. video total time(second)
	info.TotalTime = uint64(duration)

	// 4. calculate start time(second) arr
	var arr []uint64
	if info.ScoreCount == 1 {
		if uint64(duration) > info.Interval*2 { // 当时长大于两倍间隔（30s）则计算中间段的
			arr = append(arr, (uint64(duration)-info.Interval)/2)
		} else { // 当时长大于间隔但不满两倍，则直接开始进行
			arr = append(arr, 1)
			if info.Interval > uint64(duration) {
				info.Interval = uint64(duration) - 1
			}
		}
	} else if info.ScoreCount == 12 { // 当时长大于1800时认定为12段
		drop := uint64(duration) / 12
		time := (drop - info.Interval) / 2
		arr = append(arr, time)
		for i := 1; i < 12; i++ {
			time += drop
			arr = append(arr, time)
		}
	} else {
		time := (600 - info.Interval) / 2 // get the 10 minute middle
		arr = append(arr, time)
		for i := 1; i < info.ScoreCount; i++ {
			if uint64(duration)-time < 600 &&
				uint64(duration)-time > info.Interval*2 {
				time += (uint64(duration) - time - info.Interval) / 2
				arr = append(arr, time)
			} else if uint64(duration)-time > 600 {
				time += 600
				arr = append(arr, time)
			}
		}
	}

	for _, v := range arr {
		temp := CutTime{
			StartTime: v,
		}
		info.CutTimeArr = append(info.CutTimeArr, temp)
	}

	return &info, nil
}

func SimpleFilter(traceId string, path string) (*Filter2EvlInfo, string, bool) {

	// step 1. get file info by ffprobe
	mInfo, err := ffprobeMediaFile(traceId, path)
	if err != nil {
		return nil, "", false
	}

	duration, _ := strconv.ParseFloat(mInfo.Format.Duration, 64)
	bitrate, _ := strconv.ParseInt(mInfo.Format.BitRate, 10, 64)
	if duration < 30 || bitrate < 200 {
		return nil, "", false
	}
	// 	 2.2 video code type filter
	for _, stream := range mInfo.Streams {
		if stream.CodecType == VideoFlag && stream.CodecName == H264Flag {
			info, err := Filter2EvaluateInfo(mInfo)
			if err != nil {
			}
			WandH := fmt.Sprintf("%dx%d", stream.Width, stream.Height)
			return info, WandH, true
		}
	}
	return nil, "", false
}
func resolveTime(seconds uint64) (day uint64, hour uint64, minute uint64, second uint64, timeStr string) {
	day = seconds / SecondsPerDay
	hour = (seconds % SecondsPerDay) / SecondsPerHour
	minute = ((seconds % SecondsPerDay) % SecondsPerHour) / SecondsPerMinute
	second = seconds % SecondsPerMinute

	timeStr = fmt.Sprintf("%02d:%02d:%02d", hour, minute, second)
	return
}

func catFile(time uint64, prename, name string) {
	_, _, _, _, timeStr := resolveTime(time)

	cmdArguments := []string{"-y", "-i", prename, "-ss", timeStr, "-t", "15", name}

	cmd := exec.Command("ffmpeg", cmdArguments...)
	fmt.Println(cmd.String())
	var out bytes.Buffer
	var stderr bytes.Buffer
	cmd.Stdout = &out
	cmd.Stderr = &stderr
	cmd.Run()
	fmt.Println(out.String())
	fmt.Println(stderr.String())
}

func toYUV(prename, name, WandH string) {
	cmdArguments := []string{"-i", prename, "-ss", "00:00:00", "-t", "15", "-s", WandH, "-pix_fmt", "yuv420p", name}
	// ffmpeg -i 01-01.mkv  -ss 00:00:00 -t 10 -s 720x1280 -pix_fmt yuv420p 01-01.yuv
	cmd := exec.Command("ffmpeg", cmdArguments...)
	fmt.Println(cmd.String())
	var out bytes.Buffer
	var stderr bytes.Buffer
	cmd.Stdout = &out
	cmd.Stderr = &stderr
	err := cmd.Run()
	if err != nil {
		fmt.Println(err.Error())
	}
	fmt.Println(out.String())
	fmt.Println(stderr.String())
}

// 判断所给路径文件/文件夹是否存在
func Exists(path string) bool {
	_, err := os.Stat(path) //os.Stat获取文件信息
	if err != nil {
		if os.IsExist(err) {
			return true
		}
		return false
	}
	return true
}

func main() {
	args := os.Args

	path := args[1]
	info, WandH, _ := SimpleFilter("111", path)

	i := 1
	for _, v := range info.CutTimeArr {
		fmt.Println(v.StartTime)
		//fmt.Println(path)
		strs := strings.Split(path, ".mkv")
		name := strs[0] + "-" + fmt.Sprintf("%02d", i) + "new.mkv"
		fmt.Println(name)
		catFile(v.StartTime, path, name)
		//fmt.Println(WandH)
		nameYUV := strs[0] + "-" + fmt.Sprintf("%02d", i) + "new.yuv"
		flag := Exists(name)
		if flag {
			fmt.Printf("%s is exists\n", name)
			//fmt.Println(nameYUV)
			toYUV(name, nameYUV, WandH)
		} else {
			fmt.Printf("%s is not exists\n", name)
		}
		i++
	}

}
