namespace CinemaClientService.ViewModels
{
    public class SessionView
    {
        public int SessionId { get; set; }
        public DateTime StartTime { get; set; }
        public DateTime EndTime { get; set; }
        public int AvailableSeats { get; set; }
        public string Format { get; set; } = string.Empty;

        // Связанные данные
        public int HallId { get; set; }
    }
}