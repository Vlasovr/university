using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace CinemaWebService.Models
{
    public class Ticket
    {
        [BsonId] // Указывает, что это поле является идентификатором документа
        [BsonRepresentation(BsonType.ObjectId)] // Позволяет использовать строковое представление ObjectId
        public string? Id { get; set; } // Свойство для хранения _id из MongoDB опционально для него, для posgres оно должно неопциональным
        public int TicketId { get; set; } // PK
        public decimal Price { get; set; }
        public DateTime PurchaseTime { get; set; }
        public string Category { get; set; }

        // FK
        public int SessionId { get; set; }

        public int SeatId { get; set; }

        public int ClientId { get; set; }
    }
}