namespace CinemaClientService.ViewModels
{
    public class SeatView
    {
        public int SeatId { get; set; }
        public string Row { get; set; } = string.Empty;
        public bool IsElectronic { get; set; }
        
        public int HallId { get; set; }
    }
}