namespace CinemaClientService
{
    public class ReviewView
    {
        public int ReviewId { get; set; }
        public int Rating { get; set; }
        public string Comment { get; set; } = string.Empty;
        public string Source { get; set; } = string.Empty;

        // Связанные данные
        public string MovieName { get; set; } = string.Empty;
        public string ClientName { get; set; } = string.Empty;

        // Связанные ID для API
        public int MovieId { get; set; }
        public int ClientId { get; set; }
    }
}