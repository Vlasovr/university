namespace CinemaClientService.ViewModels
{
    public class TicketView
    {
        public int TicketId { get; set; }
        public decimal Price { get; set; }
        public DateTime PurchaseTime { get; set; }
        public string Category { get; set; } = string.Empty;

        // Связанные данные
        public int SessionId { get; set; }
        public int SeatId { get; set; }
        public int ClientId { get; set; }
        
        public string SessionInfo { get; set; } = string.Empty;
        public string SeatInfo { get; set; } = string.Empty;
        public string ClientInfo { get; set; } = string.Empty;
    }
}